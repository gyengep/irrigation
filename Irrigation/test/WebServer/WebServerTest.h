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


	const uint16_t port = 8080;
	std::shared_ptr<TestWebService> testWebService;
	std::unique_ptr<WebServer> webServer;

    virtual void SetUp();
    virtual void TearDown();
};
