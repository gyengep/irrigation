/*
 * Request.cpp
 *
 *  Created on: 2016.12.16.
 *      Author: 502664609
 */

#include "Common.h"
#include "Request.h"


Request::Request(void* connection, const char* url, const char* method, const char* version,
		const char* upload_data, size_t* upload_data_size) :
	connection(connection),
	url(url),
	method(method),
	version(version),
	upload_data(upload_data),
	upload_data_size(upload_data_size)
{
}

Request::~Request() {
}

void* Request::getConnection() const {
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
