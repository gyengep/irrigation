#pragma once
#include <microhttpd.h>


class Request {
	MHD_Connection* connection;
	const char* url;
	const char* method;
	const char* version;
	const char* upload_data;
	size_t* upload_data_size;

public:
	Request(MHD_Connection* connection, const char *url, const char* method, const char* version,
			const char *upload_data, size_t* upload_data_size);

	MHD_Connection* getConnection() const;
	const char* getUrl() const;
	const char* getMethod() const;
	const char* getVersion() const;
	const char* getUploadData() const;
	size_t* getUploadDataSize() const;
};
