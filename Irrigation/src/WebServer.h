
#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <list>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>


#define INVALID_SOCKET 	-1
#define SOCKET_ERROR 	-1

typedef int 			SOCKET;
typedef const char* 	LPCSTR;
typedef uint32_t 		DWORD;
typedef unsigned 		UINT;


class WebServer {
	enum {
		MAX_SOCKET = 30
	};

	typedef std::list< std::string > Request;
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

	typedef struct {
			SOCKET			socket;
			std::string		text;
			Request			request;
			bool			contentStarted;
			DWORD			contentLengthLeft;
		} Socket;

	typedef struct {
			StatusCodes		stausCode;
			ContentType		contentType;
			DWORD			messageLength;
			bool			noCache;
			char*			messageBody;
			char*			otherFields;
		} Answer;

	typedef bool (*pGetFileFunctionPtr_t)( WebServer*, const Parameters&, const Parameters&, Answer& );
	typedef std::map<std::string, pGetFileFunctionPtr_t> GetFileMap_t;
	typedef std::vector<std::string> tStringArray;

	Socket					sockets[MAX_SOCKET];
	const std::string		rootDirectory;
	const unsigned short	port;
	GetFileMap_t			m_theGetFileMap;


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

	void onSocketClose(unsigned socketID );
	bool onSocketReceive(unsigned socketID, const char* buffer, unsigned length);
	bool onSocketCreate(SOCKET socket);
	bool onRequestReceive(unsigned socketID, const Request& request);
	bool sendAnswer(unsigned socketID, const Answer& answer);

	bool getFile(const char* fileName, Answer& answer);
/*
	bool GetFile_DefaultFile( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	bool GetFile_Index_html( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	bool GetFile_SystemDateTime( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	bool GetFile_SystemConfig( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	bool GetFile_SwVersion( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	bool GetFile_FreeDisk( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	bool GetFile_TotalDisk( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	bool GetFile_SystemError( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	bool GetFile_SystemWarning( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	bool GetFile_LogFilePath( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	bool GetFile_LogFile( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	bool GetFile_StatusDBC( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );

	static bool stat_GetFile_DefaultFile( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_DefaultFile( rGetParams, rPostParams, rAnswer ); }
	static bool stat_GetFile_Index_html( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_Index_html( rGetParams, rPostParams, rAnswer ); }
	static bool stat_GetFile_SystemDateTime( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_SystemDateTime( rGetParams, rPostParams, rAnswer ); }
	static bool stat_GetFile_SystemConfig( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_SystemConfig( rGetParams, rPostParams, rAnswer ); }
	static bool stat_GetFile_SwVersion( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_SwVersion( rGetParams, rPostParams, rAnswer ); }
	static bool stat_GetFile_FreeDisk( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_FreeDisk( rGetParams, rPostParams, rAnswer ); }
	static bool stat_GetFile_TotalDisk( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_TotalDisk( rGetParams, rPostParams, rAnswer ); }
	static bool stat_GetFile_SystemError( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_SystemError( rGetParams, rPostParams, rAnswer ); }
	static bool stat_GetFile_SystemWarning( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_SystemWarning( rGetParams, rPostParams, rAnswer ); }
	static bool stat_GetFile_LogFilePath( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_LogFilePath( rGetParams, rPostParams, rAnswer ); }
	static bool stat_GetFile_LogFile( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_LogFile( rGetParams, rPostParams, rAnswer ); }
	static bool stat_GetFile_StatusDBC( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_StatusDBC( rGetParams, rPostParams, rAnswer ); }
*/
	static void setAnswer(Answer& answer, const char* text, size_t length = std::string::npos);

public:
	WebServer(const char* rootDirectory, unsigned short port);
	virtual ~WebServer();

	int  DoService( void);
};

#endif /* WEBSERVER_H_ */
