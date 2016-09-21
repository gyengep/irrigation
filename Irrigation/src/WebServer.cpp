#include "common.h"
#include "WebServer.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>
#include <arpa/inet.h>


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define HTTP_VERSION	"HTTP/1.1"
#define WEBSRV_LOG
#define NOT_USE_RESOURCE

int GetLastError() { return errno; }

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

WebServer::WebServer(unsigned short port) :
		port(port) {

	for (int ii = 0; ii < MAX_SOCKET; ++ii) {
		sockets[ii].socket = INVALID_SOCKET;
		sockets[ii].contentStarted = false;
		sockets[ii].contentLengthLeft = 0;
	}
/*
	m_theGetFileMap.insert(std::make_pair(std::string("/"), stat_GetFile_DefaultFile));
	m_theGetFileMap.insert(std::make_pair(std::string("/index.html"), stat_GetFile_Index_html));
	m_theGetFileMap.insert( std::make_pair( std::string( "/SYSTEMDATETIME" ), stat_GetFile_SystemDateTime ));
	m_theGetFileMap.insert( std::make_pair( std::string( "/SYSTEMCONFIG" ), stat_GetFile_SystemConfig ));
	m_theGetFileMap.insert( std::make_pair( std::string( "/SYSTEMERROR" ), stat_GetFile_SystemError));
	m_theGetFileMap.insert( std::make_pair( std::string( "/SYSTEMWARNING" ), stat_GetFile_SystemWarning ));
	m_theGetFileMap.insert(std::make_pair(std::string("/SWVERSION"), stat_GetFile_SwVersion));
	m_theGetFileMap.insert(std::make_pair(std::string("/FREEDISK"), stat_GetFile_FreeDisk));
	m_theGetFileMap.insert( std::make_pair( std::string( "/TOTALDISK" ), stat_GetFile_TotalDisk ));
	m_theGetFileMap.insert( std::make_pair( std::string( "/LOGFILEPATH" ), stat_GetFile_LogFilePath ));
	m_theGetFileMap.insert( std::make_pair( std::string( "/LOGFILE" ), stat_GetFile_LogFile ));
	m_theGetFileMap.insert( std::make_pair( std::string( "/STATUSDBC" ), stat_GetFile_StatusDBC ));
*/
}

WebServer::~WebServer() {
}

std::string WebServer::getStatusCodeText(StatusCodes statusCode) {

	typedef struct {
		StatusCodes statusCode;
		LPCSTR text;
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

std::string WebServer::getParamValue(const Parameters& parameters, LPCSTR name) {
	Parameters::const_iterator it = parameters.find(name);

	if (parameters.end() == it) {
		return std::string();
	}

	return it->second;
}

bool WebServer::sendAnswer(unsigned socketID, const Answer& answer) {
	bool result = true;

	if ((socketID < MAX_SOCKET) && (INVALID_SOCKET != sockets[socketID].socket)) {
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

		int sendResult;
		sendResult = send(sockets[socketID].socket, o.str().c_str(), o.str().length(), MSG_DONTROUTE);

		if ( SOCKET_ERROR != sendResult) {
			if (( NULL != answer.messageBody) && (0 != answer.messageLength)) {
				sendResult = send(sockets[socketID].socket, answer.messageBody, answer.messageLength, MSG_DONTROUTE);
			}
		} else {
#ifdef WEBSRV_LOG
			puts( "ERROR: WebServer::SendAnswer()" );
#endif
			result = false;
		}

	} else {
#ifdef WEBSRV_LOG
		puts( "ERROR: WebServer::SendAnswer()" );
#endif
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

void WebServer::onSocketClose(unsigned socketID) {
	if (socketID < MAX_SOCKET) {
		sockets[socketID].socket = INVALID_SOCKET;
		sockets[socketID].text.clear();
		sockets[socketID].request.clear();
	} else {
#ifdef WEBSRV_LOG
		puts( "ERROR: WebServer::OnSocketClose()" );
#endif
	}
}

bool WebServer::onSocketReceive(unsigned socketID, const char* buffer, unsigned length) {
	if (socketID < MAX_SOCKET) {
		std::string::size_type pos;
		sockets[socketID].text += std::string(buffer, length);

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
	} else {
#ifdef WEBSRV_LOG
		puts( "ERROR: WebServer::OnSocketReceive()" );
#endif
	}

	return true;
}

void WebServer::onSocketCreate(SOCKET socket) {
	for (unsigned socketID = 0; socketID < MAX_SOCKET; ++socketID) {
		if (INVALID_SOCKET == sockets[socketID].socket) {
			sockets[socketID].socket = socket;
			sockets[socketID].text.clear();
			return;
		}
	}

#ifdef WEBSRV_LOG
	puts( "ERROR: WebServer::OnSocketCreate()" );
#endif
}

bool WebServer::GetFileFromResource(bool bFullPath, LPCSTR lpFileName,
		Answer& rAnswer) {

#ifdef NOT_USE_RESOURCE

	return GetFile(bFullPath, lpFileName, rAnswer);

#else
	typedef
	struct tagWebFileResources {
		UINT uResouceID;
		LPCSTR lpFileName;
	}WebFileResources_t;

	static const WebFileResources_t theWebFileResources[] = {
		{	IDR_HTML_FAVICON_ICO, "/favicon.ico"},
		{	IDR_HTML_INDEX_HTML, "/index.html"},
		{	IDR_HTML_JQUERY_1_8_1_MIN_JS, "/jquery-1.8.1.min.js"},
		{	IDR_HTML_JQUERY_UI_1_8_23_CUSTOM_CSS, "/jquery-ui-1.8.23.custom.css"},
		{	IDR_HTML_JQUERY_UI_1_8_23_CUSTOM_MIN_JS, "/jquery-ui-1.8.23.custom.min.js"},
		{	IDR_HTML_KNORR_BREMSE_LOGO_JPG, "/knorr-bremse-logo.jpg"},
		{	IDR_HTML_LOADING_INDICATOR_GIF, "/loading-indicator.gif"},
		{	IDR_HTML_SITE_CSS, "/site.css"},
		{	IDR_HTML_SITE_JS, "/site.js"},
		{	IDR_HTML_IMAGES_UI_BG_FLAT_0_AAAAAA_40X100_PNG, "/images/ui-bg_flat_0_aaaaaa_40x100.png"},
		{	IDR_HTML_IMAGES_UI_BG_FLAT_75_FFFFFF_40X100_PNG, "/images/ui-bg_flat_75_ffffff_40x100.png"},
		{	IDR_HTML_IMAGES_UI_BG_GLASS_55_FBF9EE_1X400_PNG, "/images/ui-bg_glass_55_fbf9ee_1x400.png"},
		{	IDR_HTML_IMAGES_UI_BG_GLASS_65_FFFFFF_1X400_PNG, "/images/ui-bg_glass_65_ffffff_1x400.png"},
		{	IDR_HTML_IMAGES_UI_BG_GLASS_75_DADADA_1X400_PNG, "/images/ui-bg_glass_75_dadada_1x400.png"},
		{	IDR_HTML_IMAGES_UI_BG_GLASS_75_E6E6E6_1X400_PNG, "/images/ui-bg_glass_75_e6e6e6_1x400.png"},
		{	IDR_HTML_IMAGES_UI_BG_GLASS_95_FEF1EC_1X400_PNG, "/images/ui-bg_glass_95_fef1ec_1x400.png"},
		{	IDR_HTML_IMAGES_UI_BG_HIGHLIGHT_SOFT_75_CCCCCC_1X100_PNG, "/images/ui-bg_highlight-soft_75_cccccc_1x100.png"},
		{	IDR_HTML_IMAGES_UI_ICONS_222222_256X240_PNG, "/images/ui-icons_222222_256x240.png"},
		{	IDR_HTML_IMAGES_UI_ICONS_2E83FF_256X240_PNG, "/images/ui-icons_2e83ff_256x240.png"},
		{	IDR_HTML_IMAGES_UI_ICONS_454545_256X240_PNG, "/images/ui-icons_454545_256x240.png"},
		{	IDR_HTML_IMAGES_UI_ICONS_888888_256X240_PNG, "/images/ui-icons_888888_256x240.png"},
		{	IDR_HTML_IMAGES_UI_ICONS_CD0A0A_256X240_PNG, "/images/ui-icons_cd0a0a_256x240.png"},
		{	0, NULL}
	};

	for( UINT ii = 0; NULL != theWebFileResources[ii].lpFileName; ++ii ) {
		if( 0 == _stricmp( theWebFileResources[ii].lpFileName, lpFileName )) {
			CResourceReader_General theHTML( "WEBPAGE" );
			rAnswer.stausCode = HTTP_OK;
			rAnswer.messageLength = theHTML.CopyTo( theWebFileResources[ii].uResouceID, &rAnswer.messageBody );
			return true;
		}
	}

	return false;
#endif
}

bool WebServer::GetFile(bool bFullPath, LPCSTR lpFileName, Answer& rAnswer) {
	std::string strFullPath;
	FILE* f = NULL;

#ifdef NOT_USE_RESOURCE
	if (false == bFullPath) {
		const std::string m_strDirectory("C:\\Temp\\html");
		strFullPath = m_strDirectory;
	}
#endif

	strFullPath += lpFileName;

#ifdef WEBSRV_LOG
	printf( "Required file: \"%s\"\n", strFullPath.c_str() );
#endif

	f = fopen(strFullPath.c_str(), "rb");

	if ( NULL == f) {
		return false;
	}

	fseek(f, 0, SEEK_END);
	rAnswer.stausCode = HTTP_OK;
	rAnswer.messageLength = ftell(f);
	rAnswer.messageBody = new char[rAnswer.messageLength];
	rewind(f);
	fread(rAnswer.messageBody, 1, rAnswer.messageLength, f);
	fclose(f);
	return true;
}

bool WebServer::onRequestReceive(unsigned socketID, const Request& request) {
	Answer answer;
	answer.stausCode = HTTP_BAD_REQUEST;
	answer.noCache = false;
	answer.messageBody = NULL;
	answer.otherFields = NULL;
	answer.messageLength = 0;
	answer.contentType = CONTTYPE_UNKNOWN;

#ifdef WEBSRV_LOG
	TRACE( "*******************************************************" );
	for( Request::const_iterator it = request.begin(); request.end() != it; ++it ) {
		TRACE( it->c_str() );
	}
	puts( "*******************************************************" );
	for( Request::const_iterator it = request.begin(); request.end() != it; ++it ) {
		printf( "\"%s\"\n", it->c_str() );
	}
#endif // WEBSRV_LOG
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

				if (false == methodFound) {
					answer.stausCode = HTTP_METHOD_NOT_ALLOWED;
				} else {

					GetFileMap_t::iterator it = m_theGetFileMap.find(fileName);
					bool fileFound;

					if (m_theGetFileMap.end() != it) {
						fileFound = it->second(this, getParameters, postParameters, answer);
					} else {
						fileFound = GetFileFromResource(false, fileName.c_str(), answer);
					}

					if (fileFound) {
						if (CONTTYPE_UNKNOWN == answer.contentType) {
							std::string::size_type pos = fileName.find_last_of('.');
							if (std::string::npos != pos) {
								answer.contentType = getContentType(fileName.substr(pos + 1));
							}
						}
					} else {
						answer.stausCode = HTTP_NOT_FOUND;
						TRACE("WebServer: File not found: \"%s\"", fileName.c_str());
					}
				}
			}
		}
	} else {
#ifdef WEBSRV_LOG
		puts( "ERROR: WebServer::OnRequestReceive()" );
#endif
	}

	bool bResult = sendAnswer(socketID, answer);
	delete[] answer.messageBody;
	delete[] answer.otherFields;
	return bResult;
}

int WebServer::DoService(void) {
	std::cout << "int WebServer::DoService(void)" << std::endl;

	const unsigned BufferSize = 1024;
	char buffer[BufferSize];
	bool terminate = false;
	int listener = INVALID_SOCKET;
	int fdmax;
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
#ifdef WEBSRV_LOG
		printf("socket() failed: %d\n", GetLastError() );
#endif
		return 2;
	}

	std::cout << "listensocket: " << listener << std::endl;

	/*************************************************************/
	/* Allow socket descriptor to be reuseable                   */
	/*************************************************************/
	/* for setsockopt() SO_REUSEADDR, below */
	int yes = 1;

	result = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (SOCKET_ERROR == result) {
#ifdef WEBSRV_LOG
		printf("setsockopt() failed: %d\n", GetLastError() );
#endif
		close(listener);
		return 3;
	}

	std::cout << "int WebServer::DoService(void) __2__" << std::endl;

	/*************************************************************/
	/* Bind the socket                                           */
	/*************************************************************/
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port);

	result = bind(listener, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == result) {
#ifdef WEBSRV_LOG
		printf("bind() failed: %d\n", GetLastError() );
#endif
		close(listener);
		return 4;
	}

	/*************************************************************/
	/* Set the listen back log                                   */
	/*************************************************************/
	result = listen(listener, 32);
	if (SOCKET_ERROR == result) {
#ifdef WEBSRV_LOG
		printf("listen() failed: %d\n", GetLastError() );
#endif
		close(listener);
		return 5;
	}

	/*************************************************************/
	/* Add the listener to the master set                        */
	/*************************************************************/
	FD_SET(listener, &master_fd);
	onSocketCreate(listener);

	/*************************************************************/
	/* Keep track of the biggest file descriptor */
	/*************************************************************/
	fdmax = listener;

	/*************************************************************/
	/* Initialize the timeval struct to 1 second.  If no        */
	/* activity after 1 second this program will end.           */
	/*************************************************************/
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	/*************************************************************/
	/* Loop waiting for incoming connects or for incoming data   */
	/* on any of the connected sockets.                          */
	/*************************************************************/
	while (!terminate) {
		/**********************************************************/
		/* Copy the master_fd fd_set over to the working fd_set.     */
		/**********************************************************/
		memcpy(&read_fds, &master_fd, sizeof(master_fd));

		std::cout << "int WebServer::DoService(void) __3__" << std::endl;

		/**********************************************************/
		/* Call select() and wait the timeout for it to complete.   */
		/**********************************************************/
		result = select(fdmax + 1, &read_fds, NULL, NULL, &timeout);
		std::cout << "result:" << result << std::endl;

		/**********************************************************/
		/* Check to see if the select call failed.                */
		/**********************************************************/
		if ( SOCKET_ERROR == result) {
#ifdef WEBSRV_LOG
			printf( "select() failed: %d\n", GetLastError() );
#endif
			terminate = true;
			continue;
		}

		/**********************************************************/
		/* Check to see if the time out expired.                  */
		/**********************************************************/
		if (0 == result) {
			std::cout << "int WebServer::DoService(void) __4__" << std::endl;
			continue;
		}

		std::cout << "int WebServer::DoService(void) __5__" << std::endl;

		/**********************************************************/
		/* One or more descriptors are readable.  Need to         */
		/* determine which ones they are.                         */
		/**********************************************************/
		for (unsigned socketID = 0; socketID < MAX_SOCKET; ++socketID) {

			SOCKET hSocket = sockets[socketID].socket;
			if ( INVALID_SOCKET == hSocket) {
				continue;
			}

			/*******************************************************/
			/* Check to see if this descriptor is ready            */
			/*******************************************************/
			if (FD_ISSET(hSocket, &read_fds)) {

				///****************************************************/
				///* A descriptor was found that was readable - one   */
				///* less has to be looked for.  This is being done   */
				///* so that we can stop looking at the working set   */
				///* once we have found all of the descriptors that   */
				///* were ready.                                      */
				///****************************************************/
				//desc_ready -= 1;

				/****************************************************/
				/* Check to see if this is the listening socket     */
				/****************************************************/
				if (hSocket == listener) {
					SOCKET newfd;

					/**********************************************/
					/* Accept each incoming connection.  If       */
					/* accept fails with EWOULDBLOCK, then we     */
					/* have accepted all of them.  Any other      */
					/* failure on accept will cause us to end the */
					/* server.                                    */
					/**********************************************/
					struct sockaddr_in clientaddr;
					int addrlen = sizeof(clientaddr);
					newfd = accept(listener, (struct sockaddr*)&clientaddr, &addrlen);
					if ( SOCKET_ERROR == newfd) {
#ifdef WEBSRV_LOG
						printf("accept() failed: %d\n", GetLastError() );
#endif
						break;
					}

					/**********************************************/
					/* Add the new incoming connection to the     */
					/* master_fd read set                            */
					/**********************************************/
#ifdef WEBSRV_LOG
					printf("New connection from %s on socket %d\n", inet_ntoa(clientaddr.sin_addr), newfd);
#endif
					FD_SET(newfd, &master_fd);

					onSocketCreate(newfd);
				}

				/****************************************************/
				/* This is not the listening socket, therefore an   */
				/* existing connection must be readable             */
				/****************************************************/
				else {
					bool bCloseConn = false;

					/**********************************************/
					/* Receive data on this connection until the  */
					/* recv fails with EWOULDBLOCK.  If any other */
					/* failure occurs, we will close the          */
					/* connection.                                */
					/**********************************************/
					result = recv(hSocket, buffer, BufferSize, 0);
					if (result < 0) {
						if ( EWOULDBLOCK != errno) {
#ifdef WEBSRV_LOG
							printf("recv() failed: %d\n", GetLastError() );
#endif
							bCloseConn = true;
						}
					}

					/**********************************************/
					/* Check to see if the connection has been    */
					/* closed by the client                       */
					/**********************************************/
					else if (0 == result) {
#ifdef WEBSRV_LOG
						puts("Connection closed");
#endif
						bCloseConn = true;
					}

					/**********************************************/
					/* Data was recevied                          */
					/**********************************************/
					else {
						if (false == onSocketReceive(socketID, buffer, result)) {
							bCloseConn = true;
						}
					}

					/*************************************************/
					/* If the bCloseConn flag was turned on, we need */
					/* to clean up this active connection.  This     */
					/* clean up process includes removing the        */
					/* descriptor from the master_fd set and            */
					/* determining the new maximum descriptor value  */
					/* based on the bits that are still turned on in */
					/* the master_fd set.                               */
					/*************************************************/
					if (bCloseConn) {
						close(hSocket);
						FD_CLR(hSocket, &master_fd);
						onSocketClose(socketID);
					}
				} /* End of existing connection is readable */
			} /* End of if (FD_ISSET(i, &theWorkingSet)) */
		} /* End of loop through selectable descriptors */
	}

	/*************************************************************/
	/* Cleanup all of the sockets that are open                  */
	/*************************************************************/
	for (unsigned socketID = 0; socketID < MAX_SOCKET; ++socketID) {
		if ( INVALID_SOCKET != sockets[socketID].socket) {
			close(sockets[socketID].socket);
			sockets[socketID].text.clear();
			sockets[socketID].request.clear();
		}
	}

	return 0;
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
/*
bool WebServer::GetFile_DefaultFile(const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer) {
	return GetFile_Index_html(rGetParams, rPostParams, rAnswer);
}

bool WebServer::GetFile_Index_html(const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer) {

	if (rPostParams.size() == 1) {
		Parameters::const_iterator it;

		if (rPostParams.end() != (it = rPostParams.find("reboot"))) {
			if ("true" == it->second) {
				// TODO terminate
				//KBLabGetApp()->DoTerminate();
				return GetFileFromResource(false, "/index.html", rAnswer);
			}
		}
	}

#ifdef WEBSRV_LOG
	OutputDebugString( "-------------------------------\n" );

	for( Parameters::const_iterator it = rPostParams.begin(); rPostParams.end() != it; ++it ) {
		OutputDebugString( it->first.c_str() );
		OutputDebugString( " := " );
		OutputDebugString( it->second.c_str() );
		OutputDebugString( "\n" );
	}
#endif // WEBSRV_LOG

	 CXMLElement* pSectionRoot = NULL;
	 Parameters::const_iterator itDelete = rPostParams.find( "DeleteSection" );
	 Parameters::const_iterator itAdd    = rPostParams.find( "AddSection" );
	 Parameters::const_iterator itSection = rPostParams.find( "Section" );
	 Parameters::const_iterator itKeyName = rPostParams.find( "Key" );
	 Parameters::const_iterator itKeyValue = rPostParams.end();
	 Parameters::const_iterator it;

	 #ifdef WEBSRV_LOG
	 for( it = rPostParams.begin(); rPostParams.end() != it; ++it ) {
	 printf( "   %s := \"%s\"\n", it->first.c_str(), it->second.c_str() );
	 }
	 #endif

	 bool bDeleteSection = false;
	 bool bAddSection = false;

	 std::string strSection;
	 std::string strKeyName;
	 std::string strKeyValue;

	 if( rPostParams.end() != itSection ) {
	 strSection = itSection->second;
	 std::replace( strSection.begin(), strSection.end(), '.', '/' ); // replace all '.' to '/'
	 }

	 if( rPostParams.end() != itKeyName ) {
	 strKeyName = itKeyName->second;
	 itKeyValue = rPostParams.find( strKeyName );

	 if( rPostParams.end() != itKeyValue ) {
	 strKeyValue = itKeyValue->second;
	 } else {
	 #ifdef WEBSRV_LOG
	 printf( "WARNING!!! Key property does not exist: \"%s\"", strKeyName.c_str() );
	 #endif
	 }
	 }

	 if( rPostParams.end() != itDelete && 0 == strcmpi( itDelete->second.c_str(), "true" )) {
	 bDeleteSection = true;
	 }

	 if( rPostParams.end() != itAdd && 0 == strcmpi( itAdd->second.c_str(), "true" )) {
	 bAddSection = true;
	 }

	 if( strKeyName.empty() ) {

	 if( false == strSection.empty() ) {
	 pSectionRoot = KBLabGetApp()->GetSystemConfig().GetElemFromRoot( strSection.c_str(), false );
	 }
	 if( NULL == pSectionRoot ) {
	 pSectionRoot = KBLabGetApp()->GetSystemConfig().GetRoot();
	 }

	 } else {

	 std::string strKeySection;
	 std::string strParentSection;
	 std::string::size_type pos = strSection.find_last_of( '/' );

	 if( std::string::npos == pos ) {
	 strParentSection.clear();
	 strKeySection = strSection;
	 } else {
	 strParentSection = strSection.substr( 0, pos );
	 strKeySection = strSection.substr( pos + 1 );
	 }

	 CXMLElement* pSectionParent = KBLabGetApp()->GetSystemConfig().GetElemFromRoot( strParentSection.c_str(), false );
	 if( NULL != pSectionParent ) {
	 pSectionRoot = pSectionParent->FindFirst( strKeySection.c_str(), false, strKeyName.c_str(), strKeyValue.c_str() );

	 if( NULL == pSectionRoot &&  bAddSection ) {
	 pSectionRoot = pSectionParent->CreateChild( strKeySection.c_str() );
	 if( pSectionRoot ) {
	 pSectionRoot->ModifyProperty( strKeyName.c_str(), strKeyValue.c_str() );
	 }
	 }
	 }
	 }

	 if( bDeleteSection ) {
	 if( NULL != pSectionRoot ) {
	 pSectionRoot->Delete();
	 }
	 } else {
	 CXMLElement* pElement;
	 for( it = rPostParams.begin(); rPostParams.end() != it; ++it ) {
	 if( ( it == itSection ) || ( it == itKeyName ) || ( it == itKeyValue ) || ( it == itDelete ) || ( it == itAdd ) ) {
	 continue;
	 }

	 std::string strSubSection;
	 std::string strProperty;
	 std::string::size_type pos = it->first.find_last_of( '.' );

	 if( std::string::npos == pos ) {
	 strSubSection.clear();
	 strProperty = it->first;
	 pElement = pSectionRoot;
	 } else {
	 strSubSection = it->first.substr( 0, pos );
	 std::replace( strSubSection.begin(), strSubSection.end(), '.', '/' ); // replace all '.' to '/'
	 strProperty = it->first.substr( pos + 1 );
	 pElement = pSectionRoot->Find( strSubSection.c_str(), false );
	 }

	 if( NULL != pElement ) {
	 pElement->ModifyProperty( strProperty.c_str(), UriDecode( it->second ).c_str() );
	 } else {
	 #ifdef WEBSRV_LOG
	 printf( "SubSection not found: \"%s\"\n", strSubSection.c_str() );
	 #endif
	 }
	 } // for
	 }


	 KBLabGetApp()->GetSystemConfig().SetChanged();
	 KBLabGetApp()->SaveSystemConfig();

	 bool bResult = GetFileFromResource( false, "/index.html", rAnswer );
	 rAnswer.bNoCache = true;
	 return bResult;

	return false;
}

bool WebServer::GetFile_SystemDateTime(const Parameters& rGetParams,
		const Parameters& rPostParams, Answer& rAnswer) {

	 Parameters::const_iterator it = rPostParams.find( "SystemDateTime" );

	 if( rPostParams.end() != it ) {
	 SYSTEMTIME theSystemtime;
	 time_t theTime;
	 struct tm* pTM;
	 char* chDummy;

	 theTime = strtoul( it->second.c_str(), &chDummy, 10 );
	 pTM = gmtime( &theTime );

	 theSystemtime.wYear = pTM->tm_year + 1900;
	 theSystemtime.wMonth = pTM->tm_mon + 1;
	 theSystemtime.wDay = pTM->tm_mday;
	 theSystemtime.wHour = pTM->tm_hour;
	 theSystemtime.wMinute = pTM->tm_min;
	 theSystemtime.wSecond = pTM->tm_sec;
	 theSystemtime.wMilliseconds = 0;
	 SetSystemTime( &theSystemtime );
	 RTCMOSSetRTC();
	 KBLabGetApp()->UpdateBackupDate();

	 time( &theTime );
	 KBLabGetApp()->DisplayString( "System Time set to: ", ctime( &theTime ), false );
	 }

	 std::ostringstream o;
	 o << time( NULL );

	 SetAnswer( rAnswer, o.str().c_str(), o.str().length() );

	return true;
}

bool WebServer::GetFile_SystemConfig(const Parameters& rGetParams,
		const Parameters& rPostParams, Answer& rAnswer) {

	 bool bResult = GetFile( true, KBLabGetApp()->GetSystemConfigName(), rAnswer );
	 rAnswer.bNoCache = true;
	 rAnswer.eContentType = CONTTYPE_TEXT_XML;
	 return bResult;

	return false;
}

bool WebServer::GetFile_SwVersion(const Parameters& rGetParams,
		const Parameters& rPostParams, Answer& rAnswer) {
	//setAnswer( rAnswer, KBDIA_VERSION );
	return true;
}

bool WebServer::GetFile_FreeDisk(const Parameters& rGetParams,
		const Parameters& rPostParams, Answer& rAnswer) {

	 UINT nPermilleFree = 0;
	 UINT nFreeDisk = CKBDiaBufferedIO_i::GetFreeDiskKB( "C:\\", nPermilleFree );
	 char lpText[100];

	 sprintf( lpText, "%lu %lu %lu kB (%lu.%lu%%)",
	 ( nFreeDisk / (1000*1000) ),
	 ( nFreeDisk / 1000 % 1000 ) ,
	 ( nFreeDisk % 1000 ),
	 nPermilleFree / 10,
	 nPermilleFree % 10
	 );

	 SetAnswer( rAnswer, lpText );

	return true;
}


 bool
 WebServer::GetFile_TotalDisk( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) {
 UINT nFreeDisk = CKBDiaBufferedIO_i::GetTotalDiskKB( "C:\\" );
 char lpText[100];

 sprintf( lpText, "%lu %lu %lu kB",
 ( nFreeDisk / (1000*1000) ),
 ( nFreeDisk / 1000 % 1000 ) ,
 ( nFreeDisk % 1000 )
 );

 SetAnswer( rAnswer, lpText );
 return true;
 }



 bool
 WebServer::GetFile_SystemError( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) {
 WORD wErrors = KBLabGetApp()->GetReportedErrors();
 std::ostringstream o;

 for( UINT i=0; i<16; i++ ) {
 if( wErrors & (1<<i) ) {
 LPCSTR lpErrorText = CKBDiaApplication::ErrorToString( ( 1 << i ) | ( ERROR_LEVEL_ERROR << 12 ));
 if( lpErrorText ) {
 if( false == o.str().empty() ) {
 o << "<br/>";
 }

 o << lpErrorText;
 }
 }
 }

 if( o.str().empty() ) {
 o << "No errors";
 }

 SetAnswer( rAnswer, o.str().c_str(), o.str().length() );
 return true;
 }



 bool
 WebServer::GetFile_SystemWarning( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) {
 WORD wErrors = KBLabGetApp()->GetReportedWarnings();
 std::ostringstream o;

 for( UINT i=0; i<16; i++ ) {
 if( wErrors & (1<<i) ) {
 LPCSTR lpErrorText = CKBDiaApplication::WarningToString( ( 1 << i ) | ( ERROR_LEVEL_WARNING << 12 ));
 if( lpErrorText ) {
 if( false == o.str().empty() ) {
 o << "<br/>";
 }

 o << lpErrorText;
 }
 }
 }

 if( o.str().empty() ) {
 o << "No warnings";
 }

 SetAnswer( rAnswer, o.str().c_str(), o.str().length() );
 return true;
 }



 bool
 WebServer::GetFile_LogFilePath( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) {
 LPCSTR lpFileName = KBLabGetApp()->GetLogFileName();
 SetAnswer( rAnswer, lpFileName ? lpFileName : "" );
 return true;
 }



 bool
 WebServer::GetFile_LogFile( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) {
 LPCSTR lpFileName = KBLabGetApp()->GetLogFileName();
 const char* lpOtherFields = "Content-Disposition: attachment; filename=\"KBDiaDebug.log\"";
 std::ostringstream o;

 if( NULL != lpFileName ) {
 std::string line;
 std::ifstream myfile( lpFileName );
 if( myfile.is_open() ) {
 while ( myfile.good() ) {
 getline( myfile, line );
 o << line << std::endl;
 }
 myfile.close();
 } else {
 o << "ERROR: Can't open file";
 }
 }

 rAnswer.pOtherFields = new char[ strlen(lpOtherFields)+1 ];
 strcpy( rAnswer.pOtherFields, lpOtherFields );

 SetAnswer( rAnswer, o.str().c_str(), o.str().length() );
 return true;
 }



 bool
 WebServer::GetFile_StatusDBC( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) {
 CResourceReader_General theResourceDBC( "DBC" );
 const char* lpOtherFields = "Content-Disposition: attachment; filename=\"MeasboxStatus.DBC\"";
 std::string theDBC;

 theResourceDBC.CopyTo( IDR_DBC_STATUSMSG, theDBC );

 std::string::size_type nPos = theDBC.find( "$ID$");
 if( std::string::npos != nPos ) {
 char lpID[20];
 theDBC.erase( nPos, 4 );
 theDBC.insert( nPos, _ultoa( KBLabGetApp()->GetStoredStatusMsgID(), lpID, 10 ) );
 }

 rAnswer.eStausCode = HTTP_OK;
 rAnswer.bNoCache = true;
 rAnswer.eContentType = CONTTYPE_DOWNLOAD;
 rAnswer.dwMessageLength = theDBC.length();
 rAnswer.pOtherFields = new char[ strlen(lpOtherFields)+1 ];
 rAnswer.pMessageBody = new char[ rAnswer.dwMessageLength ];
 memcpy( rAnswer.pMessageBody, theDBC.c_str(), theDBC.length() );
 strcpy( rAnswer.pOtherFields, lpOtherFields );

 return true;
 } // WebServer::GetFile_StatusDBC()
 */

/*static */
void WebServer::setAnswer(Answer& answer, LPCSTR text, size_t length /*= std::string::npos*/) {
	answer.stausCode = HTTP_OK;
	answer.messageLength = (std::string::npos == length) ? strlen(text) : length;
	answer.messageBody = new char[answer.messageLength];
	answer.noCache = true;
	answer.contentType = CONTTYPE_TEXT_PLAIN;
	if (answer.messageLength > 0) {
		memcpy(answer.messageBody, text, answer.messageLength);
	}
}
