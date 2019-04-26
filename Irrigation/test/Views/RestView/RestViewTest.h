#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "CurlCallbacks/CurlCallbacks.h"
#include "Dto2Object/ProgramListSamples.h"

class IrrigationDocument;
class RestView;


class RestViewTest : public  testing::Test {
protected:
	struct Response {
		long responseCode;
	    WriteCallbackData writeCallbackData;
	    HeaderCallbackData headerCallbackData;
	};


private:
	Response __executeRequest__(const std::string& method, const std::string& url, const std::string& body, const std::string& customHeader);

protected:
	static const uint16_t port = 8080;
	std::shared_ptr<IrrigationDocument> document;

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
    void testGetPeriodicScheduler(const Dto2ObjectTest::PeriodicSchedulerSample& periodicSchedulerSample);
    void testGetWeeklyScheduler(const Dto2ObjectTest::WeeklySchedulerSample& weeklySchedulerSample);

    static void checkErrorResponse(const Response& response, long statusCode, const std::string& contentType);
    static void checkResponseWithoutBody(const Response& response, long statusCode);
    static void checkResponseWithBody(const Response& response, long statusCode, const std::string& contentType);

    static std::string createUrl(const std::string& path);
    static std::string createProgramUrl(IdType programId, const std::string& requestParameters = "");
    static std::string createStartTimeUrl(IdType programId, IdType runTimeId);
    static std::string createProgramListUrl(const std::string& requestParameters = "");
    static std::string createRunTimeListUrl(IdType programId);
    static std::string createStartTimeListUrl(IdType programId);
    static std::string createPeriodicSchedulerUrl(IdType programId);
    static std::string createWeeklySchedulerUrl(IdType programId);
    static std::string createIrrigationActionUrl();
};
