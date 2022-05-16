#include "RestViewTest.h"
#include "Mocks/MockStartTime.h"
#include "Mocks/MockStartTimeContainer.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


string RestViewTest::createStartTimeUrl(IdType programId, IdType startTimeId) {
	return createUrl("/programs/" + to_string(programId) + "/starttimes/" + to_string(startTimeId));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postStartTime) {
	Response response = executeRequest("POST", createStartTimeUrl(IdType(), IdType()), XmlWriter().save(StartTimeSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::testGetStartTime(const StartTimeListSample& startTimeListSample) {
	const IdType programId;
	const shared_ptr<Program> program = Program::Builder().setStartTimeContainer(startTimeListSample.getContainerPtr()).build();

	irrigationDocument->getPrograms().insert(programId, program);

	for (const auto& startTimeWithId : *startTimeListSample.getContainerPtr()) {
		const Response response = executeRequest("GET", createStartTimeUrl(programId, startTimeWithId.first));
		checkResponseWithBody(response, 200, "application/xml");

		EXPECT_THAT(response.curlStringWriter.getText(), Eq(XmlWriter().save(startTimeWithId.second->toStartTimeDto())));
		EXPECT_FALSE(irrigationDocument->isModified());
	}
}

TEST_F(RestViewTest, getStartTime1) {
	testGetStartTime(StartTimeListSample1());
}

TEST_F(RestViewTest, getStartTime2) {
	testGetStartTime(StartTimeListSample2());
}

TEST_F(RestViewTest, getStartTime3) {
	testGetStartTime(StartTimeListSample3());
}

TEST_F(RestViewTest, getStartTime4) {
	testGetStartTime(StartTimeListSample4());
}

TEST_F(RestViewTest, getStartTimeNotFound1) {
	Response response = executeRequest("GET", createStartTimeUrl(IdType(), IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getStartTimeNotFound2) {
	IdType programId;
	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("GET", createStartTimeUrl(programId, IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getStartTimeAcceptable) {
	const IdType programId;
	const IdType startTimeId;
	const shared_ptr<Program> program(new Program());

	irrigationDocument->getPrograms().insert(programId, program);
	program->getStartTimes().insert(startTimeId, shared_ptr<StartTime>(new StartTimeImpl()));

	Response response = executeRequest("GET", createStartTimeUrl(programId, startTimeId), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getStartTimeNotAcceptable) {
	const IdType programId;
	const IdType startTimeId;
	const shared_ptr<Program> program(new Program());

	irrigationDocument->getPrograms().insert(programId, program);
	program->getStartTimes().insert(startTimeId, shared_ptr<StartTime>(new StartTimeImpl()));

	Response response = executeRequest("GET", createStartTimeUrl(programId, startTimeId), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchStartTime) {
	const IdType programId;
	const IdType startTimeId;
	const StartTimeSample4 startTimeSample;
	const shared_ptr<MockStartTime> mockStartTime(new MockStartTime());
	const shared_ptr<Program> program(new Program());

	program->getStartTimes().insert(startTimeId, mockStartTime);
	irrigationDocument->getPrograms().insert(programId, program);

	EXPECT_CALL(*mockStartTime, updateFromStartTimeDto(startTimeSample.getDto()));

	const Response response = executeRequest("PATCH", createStartTimeUrl(programId, startTimeId), XmlWriter().save(startTimeSample.getDto()), "application/xml");
	checkResponseWithoutBody(response, 204);
	EXPECT_TRUE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, patchStartTimeNotFound) {
	const Response response = executeRequest("PATCH", createStartTimeUrl(IdType(), IdType()), XmlWriter().save(StartTimeSample1().getDto()), "application/xml");
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, patchStartTimeInvalidXml) {
	const IdType programId;
	const IdType startTimeId;
	const shared_ptr<Program> program(new Program());

	irrigationDocument->getPrograms().insert(programId, program);
	program->getStartTimes().insert(startTimeId, shared_ptr<StartTime>(new StartTimeImpl()));

	const Response response = executeRequest("PATCH", createStartTimeUrl(programId, startTimeId), "InvalidXml", "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchStartTimeInvalidContentType) {
	const IdType programId;
	const IdType startTimeId;
	const shared_ptr<Program> program(new Program());

	irrigationDocument->getPrograms().insert(programId, program);
	program->getStartTimes().insert(startTimeId, shared_ptr<StartTime>(new StartTimeImpl()));

	const Response response = executeRequest("PATCH", createStartTimeUrl(programId, startTimeId), "{ \"key\" : \"value\" }", "application/json");
	checkErrorResponse(response, 415, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, deleteStartTime) {
	const IdType programId;
	const IdType startTimeId;
	const shared_ptr<MockStartTimeContainer> mockStartTimeContainer(new MockStartTimeContainer());
	const shared_ptr<Program> program = Program::Builder().setStartTimeContainer(mockStartTimeContainer).build();

	EXPECT_CALL(*mockStartTimeContainer, erase(startTimeId));
	EXPECT_CALL(*mockStartTimeContainer, insert(_, _)).Times(AnyNumber());

	program->getStartTimes().insert(startTimeId, shared_ptr<StartTime>(new StartTimeImpl()));
	irrigationDocument->getPrograms().insert(programId, program);

	Response response = executeRequest("DELETE", createStartTimeUrl(programId, startTimeId));
	checkResponseWithoutBody(response, 200);
	EXPECT_TRUE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, deleteStartTimeNotFound) {
	Response response = executeRequest("DELETE", createStartTimeUrl(IdType(), IdType()));
	checkErrorResponse(response, 404, "application/xml");
}
