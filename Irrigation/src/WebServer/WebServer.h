
#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include "WebServer/Parameters.h"
#include "WebServer/Server.h"
#include "Utils/Tokenizer.h"

#include <list>
#include <map>
#include <vector>



class WebServer : public Server {

	FRIEND_TEST(WebServer, tokenize);
	FRIEND_TEST(WebServer, tokenizeParams);

	typedef std::list<std::string> Request;

	typedef struct {
			std::string		text;
			Request			request;
			bool			contentStarted;
			DWORD			contentLengthLeft;
		} WebSocket;

protected:

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

	WebSocket webSockets[MAX_SOCKET];

	bool onRequestReceive(unsigned socketID, const Request& request);
	bool sendAnswer(unsigned socketID, const Answer& answer);

	virtual int  onSocketCreate(SOCKET socket);
	virtual void onSocketClosed(unsigned socketID );
	virtual bool onSocketReceive(unsigned socketID, const void* buffer, unsigned length);

	virtual bool getFile(const std::string& fileName, const Parameters& getParameters, const Parameters& postParameters, Answer& answer) = 0;

	static std::string getDateStr(void);
	//static std::string getParamValue(const Parameters& parameters, const char* parameterName);
	static std::string getStatusCodeText(StatusCodes statusCodes);
	static const char* getContentTypeText(ContentType contentType);
	static ContentType getContentType(const std::string& extension);

	static bool processURI(const std::string& URI, std::string& fileName, Parameters& parameters);
	static bool tokenizeParams(const std::string& parameterText, Parameters& result);

	static std::string UriEncode( const std::string& sSrc );
	static std::string UriDecode( const std::string& sSrc );

public:
	WebServer(unsigned short port);
	virtual ~WebServer();
};


#endif /* WEBSERVER_H_ */
