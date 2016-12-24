/*
 * Request.h
 *
 *  Created on: 2016.12.16.
 *      Author: 502664609
 */

#ifndef REQUEST_H_
#define REQUEST_H_


class Request {
	void* connection;
	const char* url;
	const char* method;
	const char* version;
	const char* upload_data;
	size_t* upload_data_size;

public:
	Request(void* connection, const char *url, const char* method, const char* version,
			const char *upload_data, size_t* upload_data_size);
	virtual ~Request();

	void* getConnection() const;
	const char* getUrl() const;
	const char* getMethod() const;
	const char* getVersion() const;
	const char* getUploadData() const;
	size_t* getUploadDataSize() const;
};

#endif /* REQUEST_H_ */
