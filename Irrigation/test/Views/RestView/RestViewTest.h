#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Logic/IdType.h"
#include "Views/RestView/RestView.h"
#include "Mocks/MockFileWriter.h"
#include "Mocks/MockIrrigationDocument.h"
#include "Mocks/MockCurrentTemperature.h"
#include "Mocks/MockShutdownManager.h"
#include "Mocks/MockTemperatureHistory.h"
#include "Mocks/MockTemperatureForecast.h"
#include "TestCommon/XmlModify.h"
#include "Response.h"


class RestViewTest : public  testing::Test {

    static void checkResponseIsError(long responseCode, const AAA::Response& response);

protected:
    static const std::string defaultDateTimeFormat;
	static const uint16_t port;

	std::shared_ptr<MockIrrigationDocument> mockIrrigationDocument;
	std::shared_ptr<MockCurrentTemperature> mockCurrentTemperature;
	std::shared_ptr<MockTemperatureHistory> mockTemperatureHistory;
	std::shared_ptr<MockTemperatureForecast> mockTemperatureForecast;
	std::shared_ptr<MockShutdownManager> mockShutdownManager;
	std::shared_ptr<MockFileWriter> mockFileWriter;
	std::shared_ptr<MockFileWriterFactory> mockFileWriterFactory;

	std::shared_ptr<RestView> restView;

    virtual void SetUp();
    virtual void TearDown();

    static std::string createUrl(const std::string& path);

    static AAA::Response DELETE(const std::string& url);

    static AAA::Response GET(const std::string& url);
    static AAA::Response GET_Accept_Xml(const std::string& url);
    static AAA::Response GET_Accept_Json(const std::string& url);

    static AAA::Response PATCH_ContentType_Xml(const std::string& url, const std::string& text);
    static AAA::Response PATCH_ContentType_Json(const std::string& url, const std::string& text);

    static AAA::Response POST_ContentType_Xml(const std::string& url, const std::string& text);
    static AAA::Response POST_ContentType_Json(const std::string& url, const std::string& text);

    static void checkResponse_200_OK(const AAA::Response& response, const std::string& expectedXml);
    static void checkResponse_201_Created(const AAA::Response& response, const std::string& expectedLocation);
    static void checkResponse_204_No_Content(const AAA::Response& response);
    static void checkResponse_400_Bad_Request(const AAA::Response& response);
    static void checkResponse_404_Not_Found(const AAA::Response& response);
    static void checkResponse_405_Method_Not_Allowed(const AAA::Response& response);
    static void checkResponse_406_Not_Acceptable(const AAA::Response& response);
    static void checkResponse_415_Unsupported_Media_Type(const AAA::Response& response);
};
