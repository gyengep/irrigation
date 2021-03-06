#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Utils/CurlHeaderWriter.h"
#include "Utils/CurlStringWriter.h"
#include "Dto2Object/ProgramListSamples.h"
#include "Mocks/MockCurrentTemperature.h"
#include "Mocks/MockTemperatureHistory.h"
#include "Mocks/MockTemperatureForecast.h"

class IrrigationDocument;
class RestView;


class RestViewTest : public  testing::Test {
protected:
	struct Response {
		long responseCode;
	    CurlStringWriter curlStringWriter;
	    CurlHeaderWriter curlHeaderWriter;
	};


private:
	Response __executeRequest__(const std::string& method, const std::string& url, const std::string& body, const std::string& customHeader);

protected:
	static const uint16_t port = 8080;

	std::shared_ptr<MockCurrentTemperature> mockCurrentTemperature;
	std::shared_ptr<MockTemperatureHistory> mockTemperatureHistory;
	std::shared_ptr<MockTemperatureForecast> mockTemperatureForecast;
	std::shared_ptr<IrrigationDocument> irrigationDocument;

    virtual void SetUp();
    virtual void TearDown();

    Response executeRequest(const std::string& method, const std::string&  url);
    Response executeRequest(const std::string& method, const std::string&  url, const std::string& customHeader);
    Response executeRequest(const std::string& method, const std::string&  url, const std::string& body, const std::string& contentType);

    void testGetProgram(const Dto2ObjectTest::ProgramListSample& programListSample, const std::string& requestParameters, bool includeContainers);
    void testGetStartTime(const Dto2ObjectTest::StartTimeListSample& startTimeListSample);
    void testGetProgramList(const Dto2ObjectTest::ProgramListSample& programListSample, const std::string& requestParameters, bool includeContainers);
    void testGetRunTimeList(const Dto2ObjectTest::RunTimeListSample& runTimeListSample);
    void testPatchRunTimeList(const Dto2ObjectTest::RunTimeListSample& runTimeListSample);
    void testGetStartTimeList(const Dto2ObjectTest::StartTimeListSample& startTimeListSample);
    void testGetHotWeatherScheduler(const Dto2ObjectTest::HotWeatherSchedulerSample& hotWeatherSchedulerSample);
    void testGetPeriodicScheduler(const Dto2ObjectTest::PeriodicSchedulerSample& periodicSchedulerSample);
    void testGetTemperatureDependentScheduler(const Dto2ObjectTest::TemperatureDependentSchedulerSample& temperatureDependentSchedulerSample);
    void testGetWeeklyScheduler(const Dto2ObjectTest::WeeklySchedulerSample& weeklySchedulerSample);

    static void checkErrorResponse(const Response& response, long statusCode, const std::string& contentType);
    static void checkResponseWithoutBody(const Response& response, long statusCode);
    static void checkResponseWithBody(const Response& response, long statusCode, const std::string& contentType);
    static void checkResponseWithBody(const Response& response, long statusCode, const std::string& contentType, const std::string& body);

    static std::string createUrl(const std::string& path);
    static std::string createProgramUrl(IdType programId, const std::string& requestParameters = "");
    static std::string createStartTimeUrl(IdType programId, IdType runTimeId);
    static std::string createProgramListUrl(const std::string& requestParameters = "");
    static std::string createRunTimeListUrl(IdType programId);
    static std::string createStartTimeListUrl(IdType programId);
    static std::string createHotWeatherSchedulerUrl(IdType programId);
    static std::string createPeriodicSchedulerUrl(IdType programId);
    static std::string createTemperatureDependentSchedulerUrl(IdType programId);
    static std::string createWeeklySchedulerUrl(IdType programId);
    static std::string createIrrigationActionUrl();
    static std::string createTemperatureUrl();
    static std::string createTemperatureForecastUrl(const std::string& requestParameters = "");
    static std::string createTemperatureHistoryUrl(const std::string& requestParameters = "");
};
