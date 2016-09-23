
#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <list>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>



typedef int 			SOCKET;
typedef const char* 	LPCSTR;
typedef uint32_t 		DWORD;
typedef unsigned 		UINT;



enum {
	MAX_SOCKET = 30
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

class Server {
	const unsigned short port;
	SOCKET sockets[MAX_SOCKET];
	SOCKET listener;
	int fdmax;

public:

	class invalid_socketID : public std::exception {
	};

	Server(unsigned short port);
	virtual ~Server();

	int  doService( void);

	int send(unsigned socketID, const void* buffer, unsigned length);

	virtual void onSocketClose(unsigned socketID );
	virtual int  onSocketCreate(SOCKET socket);

	virtual bool onSocketReceive(unsigned socketID, const void* buffer, unsigned length) = 0;
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

class WebServer : public Server {

	typedef std::list<std::string> Request;
	typedef std::vector<std::string> tStringArray;

	typedef struct {
			std::string		text;
			Request			request;
			bool			contentStarted;
			DWORD			contentLengthLeft;
		} Socket;

protected:

	typedef std::map<std::string, std::string> Parameters;

	typedef enum {
			HTTP_OK					 = 200,
			HTTP_BAD_REQUEST		 = 400,
			HTTP_NOT_FOUND			 = 404,
			HTTP_METHOD_NOT_ALLOWED	 = 405,
			HTTP_VER_NOT_SUPPORTED	 = 505,
		} StatusCodes;

	typedef enum {
			CONTTYPE_TEXT_CSS,
			CONTTYPE_TEXT_HTML,
			CONTTYPE_TEXT_JS,
			CONTTYPE_TEXT_PLAIN,
			CONTTYPE_TEXT_XML,
			CONTTYPE_IMAGE_GIF,
			CONTTYPE_IMAGE_ICO,
			CONTTYPE_IMAGE_JPG,
			CONTTYPE_IMAGE_PNG,
			CONTTYPE_DOWNLOAD,
			CONTTYPE_UNKNOWN,
		} ContentType;

	typedef struct tagAnswer {
			StatusCodes		stausCode;
			ContentType		contentType;
			DWORD			messageLength;
			bool			noCache;
			char*			messageBody;
			char*			otherFields;

			tagAnswer();
			~tagAnswer();

			void set(const char* text, size_t length = std::string::npos);
			void replace(char* text, size_t length = std::string::npos);
		} Answer;

private:

	Socket sockets[MAX_SOCKET];

	bool onRequestReceive(unsigned socketID, const Request& request);
	bool sendAnswer(unsigned socketID, const Answer& answer);

	virtual int  onSocketCreate(SOCKET socket);
	virtual void onSocketClose(unsigned socketID );
	virtual bool onSocketReceive(unsigned socketID, const void* buffer, unsigned length);

	static std::string getDateStr(void);
	static std::string getParamValue(const Parameters& parameters, const char* parameterName);
	static std::string getStatusCodeText(StatusCodes statusCodes);
	static const char* getContentTypeText(ContentType contentType);
	static ContentType getContentType(const std::string& extension);

	static bool processURI(const std::string& URI, std::string& fileName, Parameters& parameters);
	static bool tokenizeParams(const std::string& parameterText, Parameters& result);
	static void tokenize(const std::string& text, tStringArray& result, char token);

	static std::string UriEncode( const std::string& sSrc );
	static std::string UriDecode( const std::string& sSrc );

public:
	WebServer(unsigned short port);
	virtual ~WebServer();

	virtual bool getFile(const std::string& fileName, const Parameters& getParameters, const Parameters& postParameters, Answer& answer) = 0;
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

class IrrigationWebServer : public WebServer {

	//typedef bool (*pGetFileFunctionPtr_t)( WebServer*, const Parameters&, const Parameters&, Answer& );
	//typedef std::map<std::string, pGetFileFunctionPtr_t> GetFileMap_t;
	//GetFileMap_t			m_theGetFileMap;

	const std::string rootDirectory;

public:
	IrrigationWebServer(const char* rootDirectory, unsigned short port);
	virtual ~IrrigationWebServer();

	virtual bool getFile(const std::string& fileName, const Parameters& getParameters, const Parameters& postParameters, Answer& answer);

/*
	bool GetFile_DefaultFile( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	static bool stat_GetFile_DefaultFile( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_DefaultFile( rGetParams, rPostParams, rAnswer ); }
*/
};

#endif /* WEBSERVER_H_ */
