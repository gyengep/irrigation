#pragma once
#include <gmock/gmock.h>
#include "WebServer/RestService.h"
#include "WebServer/HttpRequest.h"
#include "WebServer/HttpResponse.h"


class RestServiceTest : public testing::Test {
protected:

	RestService restService;

    virtual void SetUp();
    virtual void TearDown();

public:
	MOCK_METHOD2(callback1, std::unique_ptr<HttpResponse>(const HttpRequest& request, const KeyValue& pathParameters));
	MOCK_METHOD2(callback2, std::unique_ptr<HttpResponse>(const HttpRequest& request, const KeyValue& pathParameters));
	MOCK_METHOD2(callback3, std::unique_ptr<HttpResponse>(const HttpRequest& request, const KeyValue& pathParameters));
};
