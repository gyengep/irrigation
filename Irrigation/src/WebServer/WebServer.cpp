
#include "Commons/common.h"
#include "WebServer/WebServer.h"

#include <algorithm>
#include <cstring>
#include <sstream>

#include "WebServer/Parameters.h"



#define HTTP_VERSION	"HTTP/1.1"


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
		webSockets[i].contentStarted = false;
		webSockets[i].contentLengthLeft = 0;
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

	throw std::runtime_error("Invalid HTTP status code");
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

	std::string extensionCopy(extension);
	std::transform(extensionCopy.begin(), extensionCopy.end(), extensionCopy.begin(), ::toupper);

	for (unsigned i = 0; contentTypeAndTexts[i].extension; ++i) {
		if (contentTypeAndTexts[i].extension == extensionCopy) {
			return contentTypeAndTexts[i].contentType;
		}
	}

	return CONTTYPE_UNKNOWN;
}

const char* WebServer::getContentTypeText(WebServer::ContentType contentType) {

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
	const unsigned BufferSize = 500;
	char buffer[BufferSize + 1];

	time_t t = time(NULL);
	tm* timeinfo = gmtime(&t);

	strftime(buffer, BufferSize, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
	return std::string(buffer);
}
/*
std::string WebServer::getParamValue(const Parameters& parameters, const char* name) {
	Parameters::const_iterator it = parameters.find(name);

	if (parameters.end() == it) {
		return std::string();
	}

	return it->second;
}
*/
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

	LOGGER.debug("HTTP answer: %s", o.str().c_str());

	int sendResult = send(socketID, o.str().c_str(), o.str().length());

	if ( SOCKET_ERROR != sendResult) {
		if (( NULL != answer.messageBody) && (0 != answer.messageLength)) {
			sendResult = send(socketID, answer.messageBody, answer.messageLength);
		}
	}

	if ( SOCKET_ERROR == sendResult) {
		LOGGER.warning("WebServer::SendAnswer() failed");
		result = false;
	}

	return result;
}

bool WebServer::processURI(const std::string& URI, std::string& fileName, Parameters& parameters) {
	StringArray stringArray;
	//tokenize(URI, stringArray, '?');

	if (stringArray.size() < 2) {
		fileName = URI;
		parameters.clear();
	} else {
		fileName = stringArray[0];
		tokenizeParams(stringArray[1], parameters);
	}

	return true;
}

bool WebServer::tokenizeParams(const std::string& parameterText, Parameters& result) {

	result.clear();

	StringArray stringArray;
	//tokenize(parameterText, stringArray, '&');

	for (unsigned t = 0; t < stringArray.size(); t++) {
		std::string::size_type pos = stringArray[t].find('=');
		std::string key = stringArray[t].substr(0, pos);
		std::string value;

		if (std::string::npos != pos) {
			value = stringArray[t].substr(pos + 1);
		}

		if (!key.empty()) {
			result[key] = value;
		}
	}

	return true;
}

int WebServer::onSocketCreate(SOCKET socket) {
	int socketID = Server::onSocketCreate(socket);

	webSockets[socketID].text.clear();
	webSockets[socketID].request.clear();
	webSockets[socketID].contentStarted = false;
	webSockets[socketID].contentLengthLeft = 0;

	return socketID;
}

void WebServer::onSocketClosed(unsigned socketID) {
	Server::onSocketClosed(socketID);

	webSockets[socketID].text.clear();
	webSockets[socketID].request.clear();
}

bool WebServer::onSocketReceive(unsigned socketID, const void* buffer, unsigned length) {

	if (MAX_SOCKET <= socketID ) {
		throw invalid_socketID();
	}

	std::string::size_type pos;
	webSockets[socketID].text += std::string((char*)buffer, length);

	while (true) {

		if (webSockets[socketID].contentStarted) {

			if (0 != webSockets[socketID].contentLengthLeft) {
				std::string::size_type size = std::min(
						webSockets[socketID].contentLengthLeft,
						(DWORD) webSockets[socketID].text.size());

				webSockets[socketID].request.back() += webSockets[socketID].text.substr(0, size);
				webSockets[socketID].text.erase(0, size);
				webSockets[socketID].contentLengthLeft -= size;
			}

			if (0 == webSockets[socketID].contentLengthLeft) {
				bool bReceiveOK = onRequestReceive(socketID, webSockets[socketID].request);

				webSockets[socketID].request.clear();
				webSockets[socketID].contentStarted = false;
				webSockets[socketID].contentLengthLeft = 0;

				if (false == bReceiveOK) {
					return false;
				}
			}
		}

		if (std::string::npos != (pos = webSockets[socketID].text.find("\r\n"))) {
			webSockets[socketID].request.push_back(webSockets[socketID].text.substr(0, pos));
			webSockets[socketID].text.erase(0, pos + 2);

			if ("Content-Length: " == webSockets[socketID].request.back().substr(0, 16)) {
				char* chDummy = NULL;
				webSockets[socketID].contentStarted = false;
				webSockets[socketID].contentLengthLeft = strtoul(webSockets[socketID].request.back().substr(16).c_str(), &chDummy, 10);
			}

			if (webSockets[socketID].request.back().empty()) {
				if (0 < webSockets[socketID].contentLengthLeft) {
					webSockets[socketID].request.push_back(std::string());
				}

				webSockets[socketID].contentStarted = true;
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
		StringArray theRequestLineArray;
		//tokenize(request.front(), theRequestLineArray, ' ');

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

