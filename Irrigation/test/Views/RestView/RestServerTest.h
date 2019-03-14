#pragma once
#include <gmock/gmock.h>
#include "Views/RestView/RestService.h"
#include "WebServer/KeyValue.h"
#include "WebServer/WebServer.h"


class RestServerTest : public ::testing::Test {
protected:

	static const uint16_t port = 8080;
	std::shared_ptr<RestService> restService;
	std::unique_ptr<WebServer> webServer;

    virtual void SetUp();
    virtual void TearDown();

public:
	MOCK_METHOD2(onCreateProgram, std::unique_ptr<HttpResponse>(const HttpRequest& request, const KeyValue& pathParameters));
	MOCK_METHOD2(onGetProgram, std::unique_ptr<HttpResponse>(const HttpRequest& request, const KeyValue& pathParameters));
};
