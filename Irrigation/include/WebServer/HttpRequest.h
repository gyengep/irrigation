#pragma once
#include <memory>
#include <microhttpd.h>
#include <string>
#include <vector>
#include "KeyValue.h"

typedef std::vector<char> ByteBuffer;


class HttpRequest {
	MHD_Connection* connection;
	
	const std::string url;
	const std::string method;
	const std::string version;
	const std::shared_ptr<ByteBuffer> uploadData;
	
	std::unique_ptr<std::string> parametersAsText;
	std::unique_ptr<KeyValue> parameters;
	std::unique_ptr<KeyValue> headers;

	static int iterateOnValuesHeaders(void *cls, enum MHD_ValueKind kind, const char *key, const char *value);
	static int iterateOnValuesParameters(void *cls, enum MHD_ValueKind kind, const char *key, const char *value);

	typedef std::pair<KeyValue*, std::string*> ParametersHelperType;

	void initParameters() const;

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
	const std::string& getParametersAsText() const;
	const KeyValue& getParameters() const;
	const KeyValue& getHeaders() const;
};
