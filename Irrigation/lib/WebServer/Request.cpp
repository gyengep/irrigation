#include "Common.h"
#include "Request.h"


Request::Request(MHD_Connection* connection, const char* url, const char* method, const char* version,
		const char* uploadData, size_t* uploadDataSize, const Parameters& parameters) :
	connection(connection),
	url(url),
	method(method),
	version(version),
	uploadData(uploadData),
	uploadDataSize(uploadDataSize),
	parameters(parameters)
{
}

MHD_Connection* Request::getConnection() const {
	return connection;
}

const std::string& Request::getUrl() const {
	return url;
}

const std::string& Request::getMethod() const {
	return method;
}

const std::string& Request::getVersion() const {
	return version;
}

const char* Request::getUploadData() const {
	return uploadData;
}

size_t* Request::getUploadDataSize() const {
	return uploadDataSize;
}

const std::map<std::string, std::string> Request::getParameters() const {
	return parameters;
}
