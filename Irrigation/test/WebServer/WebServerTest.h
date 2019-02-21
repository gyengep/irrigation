#pragma once
#include <list>
#include <gmock/gmock.h>
#include "WebServer/HttpResponse.h"
#include "WebServer/HttpRequest.h"
#include "WebServer/WebServer.h"


class WebServerTest : public ::testing::Test {
protected:

	class TestWebService : public WebService {
	public:
		TestWebService();

		std::string lastRequestedUrl;
		std::string lastRequestedMethod;
		std::string lastRequestedVersion;
		KeyValue lastRequestedParameters;
		KeyValue lastRequestedHeaders;
		ByteBuffer lastRequestedData;

		std::string httpResponse;
		int httpResponseCode;
		KeyValue httpResponseHeaders;

		virtual std::unique_ptr<HttpResponse> onRequest(const HttpRequest& request);
	};

	struct WriteCallbackData {
		std::string text;
	};

	struct ReadCallbackData {
		std::string text;
		size_t position;

		ReadCallbackData(const std::string& text) : text(text), position(0) {}
	};

	struct HeaderCallbackData {
		std::list<std::string> headers;
	};

	const uint16_t port = 8080;
	std::shared_ptr<TestWebService> testWebService;
	std::unique_ptr<WebServer> webServer;

	std::shared_ptr<std::ostringstream> logOss;

    virtual void SetUp();
    virtual void TearDown();

public:
	static std::string createUrl(uint16_t port, const std::string& path, const KeyValue& parameters);
	static size_t writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);
	static size_t readCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);
	static size_t headerCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);
};
