
#include "common.h"
#include "Document.h"
#include "WebServer.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>
#include <arpa/inet.h>


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define HTTP_VERSION	"HTTP/1.1"
#define INVALID_SOCKET 	-1
#define SOCKET_ERROR 	-1

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
	for (unsigned i = 0; i < MAX_SOCKET; i++) {
		if (INVALID_SOCKET != sockets[i]) {
			shutdown(sockets[i], SHUT_RDWR);
			close(sockets[i]);
			sockets[i] = INVALID_SOCKET;
		}
	}
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

	LOGGER.error("ERROR: Server::OnSocketCreate()");
	return -1;
}

void Server::onSocketClose(unsigned socketID) {
	if (MAX_SOCKET <= socketID ) {
		throw invalid_socketID();
	}

	sockets[socketID] = INVALID_SOCKET;

	fdmax = listener;
	for (unsigned socketID = 0; socketID < MAX_SOCKET; ++socketID) {
		SOCKET socket = sockets[socketID];
		if (socket > fdmax) {
			fdmax = socket;
		}
	}
}

int Server::send(unsigned socketID, const void* buffer, unsigned length) {
	if (MAX_SOCKET <= socketID ) {
		throw invalid_socketID();
	}

	return ::send(sockets[socketID], buffer, length, MSG_DONTROUTE);
}

int Server::doService(void) {
	const unsigned BufferSize = 1024;
	char buffer[BufferSize];
	bool terminate = false;
	struct fd_set master_fd;
	struct fd_set read_fds;
	struct timeval timeout;
	struct sockaddr_in serveraddr;
	int result;

	/*************************************************************/
	/* Initialize the fd_set                                     */
	/*************************************************************/
	FD_ZERO(&master_fd);
	FD_ZERO(&read_fds);

	/*************************************************************/
	/* Create an AF_INET stream socket to receive incoming       */
	/* connections on                                            */
	/*************************************************************/
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == listener) {
		LOGGER.error("socket() failed: %d\n", errno);
		return 2;
	}

	/*************************************************************/
	/* Allow socket descriptor to be reuseable                   */
	/*************************************************************/
	int yes = 1;
	result = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (SOCKET_ERROR == result) {
		LOGGER.error("setsockopt() failed: %d", errno);
		close(listener);
		return 3;
	}

	/*************************************************************/
	/* Bind the socket                                           */
	/*************************************************************/
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port);

	result = bind(listener, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == result) {
		LOGGER.error("bind() failed: %d", errno);
		close(listener);
		return 4;
	}

	/*************************************************************/
	/* Set the listen back log                                   */
	/*************************************************************/
	result = listen(listener, 1);
	if (SOCKET_ERROR == result) {
		LOGGER.error("listen() failed: %d", errno);
		close(listener);
		return 5;
	}

	/*************************************************************/
	/* Add the listener to the master set                        */
	/*************************************************************/
	FD_SET(listener, &master_fd);

	/*************************************************************/
	/* Save the biggest file descriptor                          */
	/*************************************************************/
	fdmax = listener;

	/*************************************************************/
	/* Initialize the timeval struct to 1 second.  If no         */
	/* activity after 1 second this program will end.            */
	/*************************************************************/
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	/*************************************************************/
	/* Loop waiting for incoming connects or for incoming data   */
	/* on any of the connected sockets.                          */
	/*************************************************************/
	while (!terminate) {
		/**********************************************************/
		/* Copy the master_fd fd_set over to the working fd_set.  */
		/**********************************************************/
		memcpy(&read_fds, &master_fd, sizeof(master_fd));

		/**********************************************************/
		/* Call select() and wait the timeout for it to complete. */
		/**********************************************************/
		result = select(fdmax + 1, &read_fds, NULL, NULL, &timeout);

		/**********************************************************/
		/* Check to see if the select call failed.                */
		/**********************************************************/
		if ( SOCKET_ERROR == result) {
			LOGGER.error("select() failed: %d", errno);
			terminate = true;
			continue;
		}

		/**********************************************************/
		/* Check to see if the time out expired.                  */
		/**********************************************************/
		if (0 == result) {
			continue;
		}

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
				LOGGER.error("accept() failed: %d", errno);
				break;
			}

			/**********************************************/
			/* Add the new incoming connection to the     */
			/* master_fd read set                            */
			/**********************************************/

			LOGGER.debug("New connection from %s on socket %d", inet_ntoa(clientaddr.sin_addr), newfd);

			if (0 <= onSocketCreate(newfd)) {
				FD_SET(newfd, &master_fd);
			} else {
				shutdown(newfd, SHUT_RDWR);
				close(newfd);
			}
		}


		/**********************************************************/
		/* One or more descriptors are readable.  Need to         */
		/* determine which ones they are.                         */
		/**********************************************************/
		for (unsigned socketID = 0; socketID < MAX_SOCKET; ++socketID) {

			SOCKET hSocket = sockets[socketID];

			if ( INVALID_SOCKET == hSocket) {
				continue;
			}

			/*******************************************************/
			/* Check to see if this descriptor is ready            */
			/*******************************************************/
			if (FD_ISSET(hSocket, &read_fds)) {


				/**********************************************/
				/* Receive data on this connection until the  */
				/* recv fails with EWOULDBLOCK.  If any other */
				/* failure occurs, we will close the          */
				/* connection.                                */
				/**********************************************/
				if ((result = recv(hSocket, buffer, BufferSize, 0)) > 0) {
					/**********************************************/
					/* Data was recevied                          */
					/**********************************************/
					onSocketReceive(socketID, buffer, result);

				} else {
					/**********************************************/
					/* Check to see if the connection has been    */
					/* closed by the client                       */
					/**********************************************/
					if (0 == result) {
						LOGGER.info("Connection closed");
					} else {
						LOGGER.info("recv() failed: %d", errno);
					}

					close(hSocket);
					FD_CLR(hSocket, &master_fd);

					onSocketClose(socketID);
				}
			} /* End of if (FD_ISSET(i, &theWorkingSet)) */
		} /* End of loop through selectable descriptors */
	}

	/*************************************************************/
	/* Cleanup all of the sockets that are open                  */
	/*************************************************************/
	for (unsigned socketID = 0; socketID < MAX_SOCKET; ++socketID) {
		if (INVALID_SOCKET != sockets[socketID]) {
			shutdown(sockets[socketID], SHUT_RDWR);
		}
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

WebServer::Answer::tagAnswer() {
	stausCode = HTTP_BAD_REQUEST;
	contentType = CONTTYPE_UNKNOWN;
	messageLength = 0;
	noCache = false;
	messageBody = NULL;
	otherFields = NULL;
}

WebServer::Answer::~tagAnswer() {
	delete[] messageBody;
	delete[] otherFields;
}

void WebServer::Answer::set(const char* text, size_t length /*= std::string::npos*/) {
	stausCode = HTTP_OK;
	messageLength = (std::string::npos == length) ? strlen(text) : length;
	messageBody = new char[messageLength];
	noCache = true;
	contentType = CONTTYPE_UNKNOWN;
	if (messageLength > 0) {
		memcpy(messageBody, text, messageLength);
	}
}

void WebServer::Answer::replace(char* text, size_t length /*= std::string::npos*/) {
	stausCode = HTTP_OK;
	messageLength = (std::string::npos == length) ? strlen(text) : length;
	messageBody = text;
	noCache = true;
	contentType = CONTTYPE_UNKNOWN;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

WebServer::WebServer(unsigned short port) :
	Server(port)
{
	for (int i = 0; i < MAX_SOCKET; ++i) {
		sockets[i].contentStarted = false;
		sockets[i].contentLengthLeft = 0;
	}
}

WebServer::~WebServer() {
}

std::string WebServer::getStatusCodeText(StatusCodes statusCode) {

	typedef struct {
		StatusCodes statusCode;
		const char* text;
	} StatusCodeAndText;

	const static StatusCodeAndText statusCodeAndTexts[] = {
		{ HTTP_OK, "OK" },
		{ HTTP_BAD_REQUEST, "Bad Request" },
		{ HTTP_NOT_FOUND, "Not found" },
		{ HTTP_METHOD_NOT_ALLOWED, "Method not allowed" },
		{ HTTP_VER_NOT_SUPPORTED, "Version not supported" },
		{ (StatusCodes) 0xFFFF, NULL }
	};

	for (unsigned i = 0; statusCodeAndTexts[i].text; ++i) {
		if (statusCodeAndTexts[i].statusCode == statusCode) {
			return std::string(statusCodeAndTexts[i].text);
		}
	}

	return std::string();
}

WebServer::ContentType WebServer::getContentType(const std::string& extension) {

	typedef struct {
		const char* extension;
		ContentType contentType;
	} ContentTypeAndText;

	const static ContentTypeAndText contentTypeAndTexts[] = {
		{ "JS", CONTTYPE_TEXT_JS },
		{ "CSS", CONTTYPE_TEXT_CSS },
		{ "HTM", CONTTYPE_TEXT_HTML },
		{ "HTML", CONTTYPE_TEXT_HTML },
		{ "XML", CONTTYPE_TEXT_XML },
		{ "ICO", CONTTYPE_IMAGE_ICO },
		{ "JPG", CONTTYPE_IMAGE_JPG },
		{ "JPE", CONTTYPE_IMAGE_JPG },
		{ "JPEG", CONTTYPE_IMAGE_JPG },
		{ "GIF", CONTTYPE_IMAGE_GIF },
		{ "PNG", CONTTYPE_IMAGE_PNG },
		{ NULL, (ContentType) 0xFFFF }
	};

	std::string theFileExtensionCopy(extension);
	std::transform(theFileExtensionCopy.begin(), theFileExtensionCopy.end(), theFileExtensionCopy.begin(), ::toupper);

	for (unsigned i = 0; contentTypeAndTexts[i].extension; ++i) {
		if (contentTypeAndTexts[i].extension == theFileExtensionCopy) {
			return contentTypeAndTexts[i].contentType;
		}
	}

	return CONTTYPE_UNKNOWN;
}

const char* WebServer::getContentTypeText(ContentType contentType) {

	typedef struct {
		ContentType contentType;
		const char* text;
	} ContentTypeAndText;

	const static ContentTypeAndText contentTypeAndTexts[] = {
		{ CONTTYPE_TEXT_CSS, "text/css" },
		{ CONTTYPE_TEXT_HTML, "text/html" },
		{ CONTTYPE_TEXT_JS, "text/javascript" },
		{ CONTTYPE_TEXT_PLAIN, "text/plain" },
		{ CONTTYPE_TEXT_XML, "text/xml" },
		{ CONTTYPE_IMAGE_GIF, "image/gif" },
		{ CONTTYPE_IMAGE_JPG, "image/jpeg" },
		{ CONTTYPE_IMAGE_PNG, "image/png" },
		{ CONTTYPE_IMAGE_ICO, "image/x-icon" },
		{ CONTTYPE_DOWNLOAD, "application/octet-stream" },
		{ (ContentType) 0xFFFF, NULL }
	};

	for (unsigned i = 0; contentTypeAndTexts[i].text; ++i) {
		if (contentTypeAndTexts[i].contentType == contentType) {
			return contentTypeAndTexts[i].text;
		}
	}

	return NULL;
}

std::string WebServer::getDateStr(void) {
	const unsigned nSize = 500;
	char Str[nSize + 1];

	time_t t = time(NULL);
	tm* timeinfo = gmtime(&t);

	strftime(Str, nSize, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
	return std::string(Str);
}

std::string WebServer::getParamValue(const Parameters& parameters, const char* name) {
	Parameters::const_iterator it = parameters.find(name);

	if (parameters.end() == it) {
		return std::string();
	}

	return it->second;
}

bool WebServer::sendAnswer(unsigned socketID, const Answer& answer) {

	if (MAX_SOCKET <= socketID ) {
		throw invalid_socketID();
	}

	bool result = true;

	std::ostringstream o;
	const char* contentTypeText = getContentTypeText(answer.contentType);

	o << HTTP_VERSION << " " << answer.stausCode << " " << getStatusCodeText(answer.stausCode) << "\r\n";
	o << "Date: " << getDateStr() << "\r\n";
	o << "Content-Length: " << (answer.messageBody ? answer.messageLength : 0) << "\r\n";

	if ( NULL != contentTypeText) {
		o << "Content-Type: " << contentTypeText << "\r\n";
	}

	if ( NULL != answer.otherFields) {
		o << answer.otherFields << "\r\n";
	}

	if (answer.noCache) {
		o << "Cache-Control: no-cache, no-store\r\n";
	}

	o << "\r\n";

	LOGGER.debug("answer: %s", o.str().c_str());

	int sendResult;
	sendResult = send(socketID, o.str().c_str(), o.str().length());

	if ( SOCKET_ERROR != sendResult) {
		if (( NULL != answer.messageBody) && (0 != answer.messageLength)) {
			sendResult = send(socketID, answer.messageBody, answer.messageLength);
		}
	}

	if ( SOCKET_ERROR == sendResult) {
		LOGGER.error("ERROR: WebServer::SendAnswer()");
		result = false;
	}

	return result;
}

bool WebServer::processURI(const std::string& URI, std::string& fileName, Parameters& parameters) {
	tStringArray stringArray;
	tokenize(URI, stringArray, '?');

	if (stringArray.size() < 2) {
		fileName = URI;
		parameters.clear();
	} else {
		fileName = stringArray[0];
		tokenizeParams(stringArray[1], parameters);
	}

	return true;
}

void WebServer::tokenize(const std::string& text, tStringArray& result, char token) {
	result.clear();
	result.push_back(std::string());

	int t = 0;
	for (std::string::size_type i = 0; i < text.size(); i++) {
		char ch = text.at(i);

		if (ch == token) {
			result.push_back(std::string());
			t++;
			continue;
		}

		result[t] += ch;
	}
}

bool WebServer::tokenizeParams(const std::string& parameterText, Parameters& result) {
	tStringArray stringArray;
	tokenize(parameterText, stringArray, '&');

	for (unsigned t = 0; t < stringArray.size(); t++) {
		std::string::size_type pos = stringArray[t].find('=');
		std::string value;

		if (std::string::npos != pos) {
			value = stringArray[t].substr(pos + 1);
		}

		result[stringArray[t].substr(0, pos)] = value;
	}

	return true;
}

int WebServer::onSocketCreate(SOCKET socket) {

	int socketID = Server::onSocketCreate(socket);

	if (0 <= socketID) {
		sockets[socketID].text.clear();
		sockets[socketID].request.clear();
		sockets[socketID].contentStarted = false;
		sockets[socketID].contentLengthLeft = 0;
	}

	return socketID;
}

void WebServer::onSocketClose(unsigned socketID) {

	Server::onSocketClose(socketID);

	sockets[socketID].text.clear();
	sockets[socketID].request.clear();
}

bool WebServer::onSocketReceive(unsigned socketID, const void* buffer, unsigned length) {

	if (MAX_SOCKET <= socketID ) {
		throw invalid_socketID();
	}

	std::string::size_type pos;
	sockets[socketID].text += std::string((char*)buffer, length);

	while (true) {

		if (sockets[socketID].contentStarted) {

			if (0 != sockets[socketID].contentLengthLeft) {
				std::string::size_type size = std::min(
						sockets[socketID].contentLengthLeft,
						(DWORD) sockets[socketID].text.size());

				sockets[socketID].request.back() += sockets[socketID].text.substr(0, size);
				sockets[socketID].text.erase(0, size);
				sockets[socketID].contentLengthLeft -= size;
			}

			if (0 == sockets[socketID].contentLengthLeft) {
				bool bReceiveOK = onRequestReceive(socketID, sockets[socketID].request);

				sockets[socketID].request.clear();
				sockets[socketID].contentStarted = false;
				sockets[socketID].contentLengthLeft = 0;

				if (false == bReceiveOK) {
					return false;
				}
			}
		}

		if (std::string::npos != (pos = sockets[socketID].text.find("\r\n"))) {
			sockets[socketID].request.push_back(sockets[socketID].text.substr(0, pos));
			sockets[socketID].text.erase(0, pos + 2);

			if ("Content-Length: " == sockets[socketID].request.back().substr(0, 16)) {
				char* chDummy = NULL;
				sockets[socketID].contentStarted = false;
				sockets[socketID].contentLengthLeft = strtoul(sockets[socketID].request.back().substr(16).c_str(), &chDummy, 10);
			}

			if (sockets[socketID].request.back().empty()) {
				if (0 < sockets[socketID].contentLengthLeft) {
					sockets[socketID].request.push_back(std::string());
				}

				sockets[socketID].contentStarted = true;
			}

			continue;
		}

		return true;
	}

	return true;
}

bool WebServer::onRequestReceive(unsigned socketID, const Request& request) {
	Answer answer;

	LOGGER.trace( "*******************************************************" );
	for( Request::const_iterator it = request.begin(); request.end() != it; ++it ) {
		LOGGER.trace("%s", it->c_str());
	}

	if (!request.empty()) {
		tStringArray theRequestLineArray;
		tokenize(request.front(), theRequestLineArray, ' ');

		if (theRequestLineArray.size() == 3) {
			if ( HTTP_VERSION != theRequestLineArray[2]) {
				answer.stausCode = HTTP_VER_NOT_SUPPORTED;
			} else {
				std::string fileName;
				Parameters getParameters, postParameters;
				bool methodFound = false;

				if ("GET" == theRequestLineArray[0]) {
					methodFound = true;
					processURI(theRequestLineArray[1], fileName, getParameters);

				} else if ("POST" == theRequestLineArray[0]) {
					methodFound = true;
					processURI(theRequestLineArray[1], fileName, getParameters);

					Request::const_iterator it;
					for (it = request.begin(); request.end() != it; ++it) {
						if (it->empty()) {
							++it;
							if (request.end() != it) {
								tokenizeParams(*it, postParameters);
							}
							break;
						}
					}
				}

				if (methodFound) {
					LOGGER.debug("File request: %s", fileName.c_str());

					if (getFile(fileName, getParameters, postParameters, answer)) {
						if (CONTTYPE_UNKNOWN == answer.contentType) {
							std::string::size_type pos = fileName.find_last_of('.');
							if (std::string::npos != pos) {
								answer.contentType = getContentType(fileName.substr(pos + 1));
							}
						}
					} else {
						answer.stausCode = HTTP_NOT_FOUND;
						LOGGER.warning("WebServer: File not found: \"%s\"", fileName.c_str());
					}
				} else {
					answer.stausCode = HTTP_METHOD_NOT_ALLOWED;
				}
			}
		}
	} else {
		LOGGER.error("ERROR: WebServer::OnRequestReceive()");
	}

	return sendAnswer(socketID, answer);
}

const char HEX2DEC[256] = {
/*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
/* 0 */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* 1 */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* 2 */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* 3 */0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,

/* 4 */-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* 5 */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* 6 */-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* 7 */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

/* 8 */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* 9 */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* A */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* B */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

/* C */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* D */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* E */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
/* F */-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

// Only alphanum is safe.
const char SAFE[256] = {
/*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
/* 0 */0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 1 */0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 2 */0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 3 */1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,

/* 4 */0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 5 */1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
/* 6 */0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 7 */1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,

/* 8 */0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 9 */0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* A */0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* B */0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

/* C */0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* D */0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* E */0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* F */0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/*static*/std::string WebServer::UriEncode(const std::string & sSrc) {
	const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
	const unsigned char * pSrc = (const unsigned char *) sSrc.c_str();
	const int SRC_LEN = sSrc.length();
	unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
	unsigned char * pEnd = pStart;
	const unsigned char * const SRC_END = pSrc + SRC_LEN;

	for (; pSrc < SRC_END; ++pSrc) {
		if (SAFE[*pSrc]) {
			*pEnd++ = *pSrc;
		} else {
			// escape this char
			*pEnd++ = '%';
			*pEnd++ = DEC2HEX[*pSrc >> 4];
			*pEnd++ = DEC2HEX[*pSrc & 0x0F];
		}
	}

	std::string sResult((char *) pStart, (char *) pEnd);
	delete[] pStart;
	return sResult;
}

/*static*/std::string WebServer::UriDecode(const std::string & sSrc) {
	// Note from RFC1630:  "Sequences which start with a percent sign
	// but are not followed by two hexadecimal characters (0-9, A-F) are reserved
	// for future extension"

	const unsigned char * pSrc = (const unsigned char *) sSrc.c_str();
	const int SRC_LEN = sSrc.length();
	const unsigned char * const SRC_END = pSrc + SRC_LEN;
	const unsigned char * const SRC_LAST_DEC = SRC_END - 2; // last decodable '%'

	char * const pStart = new char[SRC_LEN];
	char * pEnd = pStart;

	while (pSrc < SRC_LAST_DEC) {
		if (*pSrc == '%') {
			char dec1, dec2;
			if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
					&& -1 != (dec2 = HEX2DEC[*(pSrc + 2)])) {
				*pEnd++ = (dec1 << 4) + dec2;
				pSrc += 3;
				continue;
			}
		}

		*pEnd++ = *pSrc++;
	}

	// the last 2- chars
	while (pSrc < SRC_END)
		*pEnd++ = *pSrc++;

	std::string sResult(pStart, pEnd);
	delete[] pStart;
	return sResult;
}

 ////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////


IrrigationWebServer::IrrigationWebServer(const char* rootDirectory, unsigned short port, const Document* document) :
	WebServer(port),
	rootDirectory(rootDirectory),
	document(document)
{
//	m_theGetFileMap.insert(std::make_pair(std::string("/"), stat_GetFile_DefaultFile));
}

IrrigationWebServer::~IrrigationWebServer() {
}


void IrrigationWebServer::addPrograms(const ProgramContainer& programs, Teng::FragmentList_t& programsFragment) {
    for (auto it = programs.container().begin(); programs.container().end() != it; ++it) {
        const Program* program = it->second;
    	Teng::Fragment_t& programFragment = programsFragment.addFragment();

    	programFragment.addVariable("ID", (long)it->first);
    	programFragment.addVariable("name", program->getName());
    	addRunTimes(program->runTimes(), programFragment.addFragmentList("runTimes"));
    	addStartTimes(program->startTimes(), programFragment.addFragmentList("startTimes"));
    }
}

void IrrigationWebServer::addRunTimes(const RunTimeContainer& runTimes, Teng::FragmentList_t& runTimesFragment) {
    for (auto it = runTimes.container().begin(); runTimes.container().end() != it; ++it) {
    	Teng::Fragment_t& runTimeFragment = runTimesFragment.addFragment();

    	runTimeFragment.addVariable("ID", (long)it->first);
    	runTimeFragment.addVariable("minutes", (long)it->second);
    }
}

void IrrigationWebServer::addStartTimes(const StartTimeContainer& startTimes, Teng::FragmentList_t& startTimesFragment) {
    for (auto it = startTimes.container().begin(); startTimes.container().end() != it; ++it) {
    	Teng::Fragment_t& startTimeFragment = startTimesFragment.addFragment();

    	startTimeFragment.addVariable("ID", (long)it->first);
    	startTimeFragment.addVariable("minutes", (long)it->second);
    }
}

void IrrigationWebServer::updateTemplateEngine() {
    // Create Teng engine
    Teng::Teng_t teng("", Teng::Teng_t::LM_LOG_TO_OUTPUT);

    // Root data fragment
    Teng::Fragment_t root;

    addPrograms(document->programs(), root.addFragmentList("programs"));
}

bool IrrigationWebServer::getFile(const std::string& fileName, const Parameters& getParameters, const Parameters& postParameters, Answer& answer) {
	std::string strFullPath = rootDirectory + fileName;

	FILE* f = fopen(strFullPath.c_str(), "rb");
	if (NULL == f) {
		return false;
	}

	long messageLength;
	char* messageBody;

	fseek(f, 0, SEEK_END);
	messageLength = ftell(f);
	messageBody = new char[messageLength];
	rewind(f);
	fread(messageBody, 1, messageLength, f);
	fclose(f);

	answer.replace(messageBody, messageLength);

	return true;

/*
	GetFileMap_t::iterator it = m_theGetFileMap.find(fileName);
	bool fileFound;

	if (m_theGetFileMap.end() != it) {
		fileFound = it->second(this, getParameters, postParameters, answer);
	} else {
		fileFound = readFileFromDisk(fileName, answer);
	}

	return fileFound;
*/
}

