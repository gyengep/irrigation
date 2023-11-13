#pragma once
#include <memory>
#include <string>
#include "WebServer/KeyValue.h"
#include "WebServer/WebServerException.h"
#include "ErrorWriter.h"


class HTTP_Exception : public WebServerException {
public:
	HTTP_Exception(
		const std::shared_ptr<ErrorWriter>& errorWriter,
		unsigned statusCode,
		const std::string& genericMessage,
		const std::string& detailedMessage
	) :
		WebServerException(
				statusCode,
				genericMessage,
				errorWriter->toString(statusCode, genericMessage, detailedMessage),
				KeyValue({{"Content-Type", errorWriter->contentType()}})
			)
	{
	}
};


class HTTP_400_Bad_Request : public HTTP_Exception {
public:
	HTTP_400_Bad_Request(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		HTTP_Exception(errorWriter, 400, "Bad Request", message)
	{
	}
};

class HTTP_404_Not_Found : public HTTP_Exception {
public:
	HTTP_404_Not_Found(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		HTTP_Exception(errorWriter, 404, "Not Found", message)
	{
	}
};

class HTTP_405_Method_Not_Allowed : public HTTP_Exception {
public:
	HTTP_405_Method_Not_Allowed(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		HTTP_Exception(errorWriter, 405, "Method Not Allowed", message)
	{
	}
};

class HTTP_406_Not_Acceptable : public HTTP_Exception {
public:
	HTTP_406_Not_Acceptable(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		HTTP_Exception(errorWriter, 406, "Not Acceptable", message)
	{
	}
};

class HTTP_415_Unsupported_Media_Type : public HTTP_Exception {
public:
	HTTP_415_Unsupported_Media_Type(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		HTTP_Exception(errorWriter, 415, "Unsupported Media Type", message)
	{
	}
};

class HTTP_422_Unprocessable_Content : public HTTP_Exception {
public:
	HTTP_422_Unprocessable_Content(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		HTTP_Exception(errorWriter, 422, "Unprocessable Content", message)
	{
	}
};

class HTTP_500_Internal_Server_Error : public HTTP_Exception {
public:
	HTTP_500_Internal_Server_Error(const std::shared_ptr<ErrorWriter>& errorWriter, const std::string& message) :
		HTTP_Exception(errorWriter, 500, "Internal Server Error", message)
	{
	}
};

