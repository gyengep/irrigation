#pragma once
#include <map>
#include <memory>
#include <microhttpd.h>
#include <string>
#include <vector>

typedef std::map<std::string, std::string> Parameters;
typedef std::vector<char> ByteBuffer;


class HttpRequest {
	MHD_Connection* connection;
	const std::string url;
	const std::string method;
	const std::string version;
	const std::shared_ptr<ByteBuffer> uploadData;
	std::unique_ptr<Parameters> parameters;
	std::unique_ptr<Parameters> headers;

	static int iterateOnValues(void *cls, enum MHD_ValueKind kind, const char *key, const char *value);

public:
	HttpRequest(const HttpRequest&) = delete;
	HttpRequest(HttpRequest&&) = delete;
	HttpRequest(MHD_Connection* connection, const char* version, const char* method, const char *url,
			const std::shared_ptr<ByteBuffer>& uploadData);

	const HttpRequest& operator= (const HttpRequest&) = delete;
	const HttpRequest& operator= (HttpRequest&&) = delete;

	const std::string& getUrl() const { return url; }
	const std::string& getMethod() const { return method; }
	const std::string& getVersion() const { return version; }
	const std::shared_ptr<ByteBuffer>& getUploadData() const { return uploadData; }
	const Parameters& getParameters() const;
	const Parameters& getHeaders() const;
};
