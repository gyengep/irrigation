#pragma once
#include <map>
#include <microhttpd.h>


class Request {
public:

	typedef std::map<std::string, std::string> Parameters;

private:
	MHD_Connection* connection;
	const std::string url;
	const std::string method;
	const std::string version;
	const char* uploadData;
	size_t* uploadDataSize;
	const Parameters& parameters;

	// disable copy constructor and operator
	Request(const Request&);
	const Request& operator= (const Request&);

public:
	Request(MHD_Connection* connection, const char *url, const char* method, const char* version,
			const char *upload_data, size_t* upload_data_size, const Parameters& parameters);

	MHD_Connection* getConnection() const;
	const std::string& getUrl() const;
	const std::string& getMethod() const;
	const std::string& getVersion() const;
	const char* getUploadData() const;
	size_t* getUploadDataSize() const;
	const std::map<std::string, std::string> getParameters() const;
};
