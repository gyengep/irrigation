
#include "Commons/common.h"
#include "WebServer/Server.h"

#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <sys/socket.h>



Server::Server(unsigned short port) :
	port(port),
	listener(INVALID_SOCKET),
	fdmax(0)
{
	for (unsigned i = 0; i < MAX_SOCKET; i++) {
		sockets[i] = INVALID_SOCKET;
	}
}

Server::~Server() {
	cleanup();
}

int Server::onSocketCreate(SOCKET socket) {
	for (unsigned socketID = 0; socketID < MAX_SOCKET; ++socketID) {
		if (INVALID_SOCKET == sockets[socketID]) {

			if (socket > fdmax) {
				fdmax = socket;
			}

			sockets[socketID] = socket;
			return socketID;
		}
	}

	throw socket_exception("No more free socket");
}

void Server::onSocketClosed(unsigned socketID) {
	if (MAX_SOCKET > socketID ) {
		sockets[socketID] = INVALID_SOCKET;

		fdmax = listener;
		for (unsigned socketID = 0; socketID < MAX_SOCKET; ++socketID) {
			SOCKET socket = sockets[socketID];
			if (socket > fdmax) {
				fdmax = socket;
			}
		}
	}

	throw invalid_socketID();
}

int Server::send(unsigned socketID, const void* buffer, unsigned length) {
	if (MAX_SOCKET > socketID ) {
		return ::send(sockets[socketID], buffer, length, MSG_DONTROUTE);
	}

	throw invalid_socketID();
}

void Server::initFdSets() {
	/*************************************************************/
	/* Initialize the fd_set                                     */
	/*************************************************************/
	FD_ZERO(&master_fd);
	FD_ZERO(&read_fds);
}

void Server::initTimeout() {
	/*************************************************************/
	/* Initialize the timeval struct to 1 second.  If no         */
	/* activity after 1 second this program will end.            */
	/*************************************************************/
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
}

void Server::initListener() {
	int result;

	/*************************************************************/
	/* Create an AF_INET stream socket to receive incoming       */
	/* connections on                                            */
	/*************************************************************/
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == listener) {
		throw socket_exception("socket() failed: " + std::to_string(errno));
	}


	/*************************************************************/
	/* Allow socket descriptor to be reuseable                   */
	/*************************************************************/
	int yes = 1;
	result = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (SOCKET_ERROR == result) {
		throw socket_exception("setsockopt() failed: " + std::to_string(errno));
	}


	/*************************************************************/
	/* Bind the socket                                           */
	/*************************************************************/
	struct sockaddr_in serveraddr;

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port);

	result = bind(listener, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == result) {
		throw socket_exception("bind() failed: " + std::to_string(errno));
	}


	/*************************************************************/
	/* Set the listen back log                                   */
	/*************************************************************/
	result = listen(listener, 1);
	if (SOCKET_ERROR == result) {
		throw socket_exception("listen() failed: " + std::to_string(errno));
	}


	/*************************************************************/
	/* Add the listener to the master set                        */
	/*************************************************************/
	FD_SET(listener, &master_fd);


	/*************************************************************/
	/* Save the largest file descriptor                          */
	/*************************************************************/
	fdmax = listener;
}


bool Server::select() {

	/**********************************************************/
	/* Copy the master_fd fd_set over to the working fd_set.  */
	/**********************************************************/
	memcpy(&read_fds, &master_fd, sizeof(master_fd));

	int result = ::select(fdmax + 1, &read_fds, NULL, NULL, &timeout);

	/**********************************************************/
	/* Check to see if the select call failed.                */
	/**********************************************************/
	if (SOCKET_ERROR == result) {
		throw socket_exception("select() failed: " + std::to_string(errno));
	}

	return (0 < result);
}

void Server::checkListeningSocket() {
	/****************************************************/
	/* Check to see if this is the listening socket     */
	/****************************************************/
	if (FD_ISSET(listener, &read_fds)) {
		struct sockaddr_in clientaddr;
		SOCKET newfd;

		/**********************************************/
		/* Accept each incoming connection.  If       */
		/* accept fails with EWOULDBLOCK, then we     */
		/* have accepted all of them.  Any other      */
		/* failure on accept will cause us to end the */
		/* server.                                    */
		/**********************************************/
		socklen_t addrlen = sizeof(clientaddr);

		// TODO le kellene kezelni, ha több mint MAX_SOCKET socket van
		newfd = accept(listener, (struct sockaddr*)&clientaddr, &addrlen);
		if (SOCKET_ERROR == newfd) {
			throw socket_exception("accept() failed: " + std::to_string(errno));
		}

		/**********************************************/
		/* Add the new incoming connection to the     */
		/* master_fd read set                            */
		/**********************************************/

		LOGGER.info("New connection from %s on socket %d", inet_ntoa(clientaddr.sin_addr), newfd);

		try {
			onSocketCreate(newfd);
			FD_SET(newfd, &master_fd);
		} catch (socket_exception& e) {
			LOGGER.warning(e.what());
			shutDownAndClose(newfd);
		}
	}
}

void Server::checkClientSockets() {

	const unsigned BufferSize = 1024;
	char buffer[BufferSize];

	/**********************************************************/
	/* One or more descriptors are readable.  Need to         */
	/* determine which ones they are.                         */
	/**********************************************************/
	for (unsigned socketID = 0; socketID < MAX_SOCKET; ++socketID) {

		SOCKET hSocket = sockets[socketID];

		if (INVALID_SOCKET == hSocket) {
			continue;
		}

		/*******************************************************/
		/* Check to see if this descriptor is ready            */
		/*******************************************************/
		if (FD_ISSET(hSocket, &read_fds)) {
			int readed;

			/**********************************************/
			/* Receive data on this connection until the  */
			/* recv fails with EWOULDBLOCK.  If any other */
			/* failure occurs, we will close the          */
			/* connection.                                */
			/**********************************************/
			if ((readed = recv(hSocket, buffer, BufferSize, 0)) > 0) {
				/**********************************************/
				/* Data was recevied                          */
				/**********************************************/
				onSocketReceive(socketID, buffer, readed);

			} else {
				/**********************************************/
				/* Check to see if the connection has been    */
				/* closed by the client                       */
				/**********************************************/
				if (0 == readed) {
					LOGGER.info("Connection closed");
				} else {
					LOGGER.warning("recv() failed: %d", errno);
				}

				shutDownAndClose(hSocket);
				FD_CLR(hSocket, &master_fd);

				onSocketClosed(socketID);
			}
		}
	}
}

void Server::cleanup() {

	if (INVALID_SOCKET != listener) {
		shutDownAndClose(listener);
		listener = INVALID_SOCKET;
	}

	/*************************************************************/
	/* Cleanup all of the sockets that are open                  */
	/*************************************************************/
	for (unsigned socketID = 0; socketID < MAX_SOCKET; socketID++) {
		if (INVALID_SOCKET != sockets[socketID]) {
			shutDownAndClose(sockets[socketID]);
			sockets[socketID] = INVALID_SOCKET;
		}
	}
}

void Server::stop() {
	terminate = true;
}

void Server::doService(void) {

	initFdSets();
	initTimeout();

	try {
		initListener();

		terminate = false;
		while (!terminate) {


			/**********************************************************/
			/* Check to see if the time out expired.                  */
			/**********************************************************/
			if (select()) {
				checkListeningSocket();
				checkClientSockets();
			}

		}
	} catch (socket_exception& e) {
		LOGGER.error(e.what());
	}

	cleanup();
}

/*static*/ void Server::shutDownAndClose(SOCKET socket) {
	shutdown(socket, SHUT_RDWR);
	close(socket);
}
