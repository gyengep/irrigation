#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "CurlCallbacks/CurlCallbacks.h"
#include "Dto2Object/ProgramSamples.h"
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

	static const uint16_t port = 8080;
	std::shared_ptr<IrrigationDocument> document;

    virtual void SetUp();
    virtual void TearDown();

    Response executeRequest(const std::string& method, const std::string&  url, const std::string& sendData = std::string());

    void testGetProgram(const IdType& programId, const ProgramDTO& programDto);
    void testGetRunTime(const IdType& programId, const IdType& runTimeId, const RunTimeDTO& runTimeDto);
    void testGetStartTime(const IdType& programId, const IdType& startTimeId, const StartTimeDTO& startTimeDto);
    void testGetProgramList(const std::list<ProgramDTO>& programDtoList);
    void testGetRunTimeList(const IdType& programId, const std::list<RunTimeDTO>& runTimeDtoList);
    void testGetStartTimeList(const IdType& programId, const std::list<StartTimeDTO>& startTimeDtoList);

    static std::string createUrl(const std::string& path);
    static std::string createProgramUrl(IdType programId);
    static std::string createRunTimeUrl(IdType programId, IdType runTimeId);
    static std::string createStartTimeUrl(IdType programId, IdType runTimeId);
    static std::string createProgramListUrl();
    static std::string createRunTimeListUrl(IdType programId);
    static std::string createStartTimeListUrl(IdType programId);
};
