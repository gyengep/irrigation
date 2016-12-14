
#ifndef SERVER_H_
#define SERVER_H_


#include <atomic>



typedef int 			SOCKET;
typedef const char* 	LPCSTR;
typedef uint32_t 		DWORD;
typedef unsigned 		UINT;


#define INVALID_SOCKET 	-1
#define SOCKET_ERROR 	-1

enum {
	MAX_SOCKET = 30
};

class socket_exception : public std::runtime_error {
public:
	socket_exception(const std::string& what_arg) : std::runtime_error(what_arg) {}
};

class invalid_socketID : public std::runtime_error {
public:
	invalid_socketID() : std::runtime_error("Invalid SocketID") {}
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

class Server {
	const unsigned short port;
	SOCKET sockets[MAX_SOCKET];
	SOCKET listener;
	int fdmax;
	std::atomic_bool terminate;

	struct timeval timeout;
	struct fd_set master_fd;
	struct fd_set read_fds;

	void initTimeout();
	void initFdSets();
	void initListener();
	void checkListeningSocket();
	void checkClientSockets();
	void cleanup();
	bool select();

	static void shutDownAndClose(SOCKET socket);

protected:
	virtual int  onSocketCreate(SOCKET socket);
	virtual void onSocketClosed(unsigned socketID );
	virtual bool onSocketReceive(unsigned socketID, const void* buffer, unsigned length) = 0;

public:

	Server(unsigned short port);
	virtual ~Server();

	void doService(void);
	void stop();

	int send(unsigned socketID, const void* buffer, unsigned length);
};


#endif /* SERVER_H_ */
