#include "Common.h"
#include "Request.h"


Request::Request(MHD_Connection* connection, const char* url, const char* method, const char* version,
		const char* upload_data, size_t* upload_data_size) :
	connection(connection),
	url(url),
	method(method),
	version(version),
	upload_data(upload_data),
	upload_data_size(upload_data_size)
{
}

MHD_Connection* Request::getConnection() const {
	return connection;
}

const char* Request::getUrl() const {
	return url;
}

const char* Request::getMethod() const {
	return method;
}

const char* Request::getVersion() const {
	return version;
}

const char* Request::getUploadData() const {
	return upload_data;
}

size_t* Request::getUploadDataSize() const {
	return upload_data_size;
}
