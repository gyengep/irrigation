#include "RestViewTest.h"
#include "Mocks/MockStartTime.h"
#include "Mocks/MockStartTimeContainer.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


std::string RestViewTest::createStartTimeUrl(IdType programId, IdType startTimeId) {
	return createUrl("/programs/" + to_string(programId) + "/starttimes/" + to_string(startTimeId));
}

void RestViewTest::testGetStartTime(const IdType& programId, const IdType& startTimeId, const StartTimeDTO& startTimeDTO) {
	Response response = executeRequest("GET", createStartTimeUrl(programId, startTimeId));
	checkResponseWithBody(response, 200, "application/xml");
	EXPECT_THAT(response.writeCallbackData.text, Eq(XmlWriter().save(startTimeDTO)));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postStartTime) {
	Response response = executeRequest("POST", createStartTimeUrl(IdType(), IdType()), XmlWriter().save(StartTimeSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, getStartTime) {
	const IdType programId;
	const shared_ptr<Program> program(new Program());

	program->getStartTimes().insert(1, StartTimeSample1().getObject());
	program->getStartTimes().insert(2, StartTimeSample2().getObject());
	program->getStartTimes().insert(3, StartTimeSample3().getObject());
	program->getStartTimes().insert(4, StartTimeSample4().getObject());

	document->getPrograms().insert(programId, program);

	testGetStartTime(programId, 1, StartTimeSample1().getDto());
	testGetStartTime(programId, 2, StartTimeSample2().getDto());
	testGetStartTime(programId, 3, StartTimeSample3().getDto());
	testGetStartTime(programId, 4, StartTimeSample4().getDto());
}

TEST_F(RestViewTest, getStartTimeNotFound1) {
	Response response = executeRequest("GET", createStartTimeUrl(IdType(), IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getStartTimeNotFound2) {
	IdType programId;
	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("GET", createStartTimeUrl(programId, IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getStartTimeAcceptable) {
	const IdType programId;
	const IdType startTimeId;
	const shared_ptr<Program> program(new Program());

	document->getPrograms().insert(programId, program);
	program->getStartTimes().insert(startTimeId, shared_ptr<StartTime>(new StartTime));

	Response response = executeRequest("GET", createStartTimeUrl(programId, startTimeId), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getStartTimeNotAcceptable) {
	const IdType programId;
	const IdType startTimeId;
	const shared_ptr<Program> program(new Program());

	document->getPrograms().insert(programId, program);
	program->getStartTimes().insert(startTimeId, shared_ptr<StartTime>(new StartTime));

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
	document->getPrograms().insert(programId, program);

	EXPECT_CALL(*mockStartTime, updateFromStartTimeDto(startTimeSample.getDto()));

	const Response response = executeRequest("PATCH", createStartTimeUrl(programId, startTimeId), XmlWriter().save(startTimeSample.getDto()), "application/xml");
	checkResponseWithoutBody(response, 204);
}

TEST_F(RestViewTest, patchStartTimeNotFound) {
	const Response response = executeRequest("PATCH", createStartTimeUrl(IdType(), IdType()), XmlWriter().save(StartTimeSample1().getDto()), "application/xml");
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, patchStartTimeInvalidXml) {
	const IdType programId;
	const IdType startTimeId;
	const shared_ptr<Program> program(new Program());

	document->getPrograms().insert(programId, program);
	program->getStartTimes().insert(startTimeId, shared_ptr<StartTime>(new StartTime()));

	const Response response = executeRequest("PATCH", createStartTimeUrl(programId, startTimeId), "InvalidXml", "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchStartTimeInvalidContentType) {
	const IdType programId;
	const IdType startTimeId;
	const shared_ptr<Program> program(new Program());

	document->getPrograms().insert(programId, program);
	program->getStartTimes().insert(startTimeId, shared_ptr<StartTime>(new StartTime()));

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

	program->getStartTimes().insert(startTimeId, shared_ptr<StartTime>(new StartTime()));
	document->getPrograms().insert(programId, program);

	Response response = executeRequest("DELETE", createStartTimeUrl(programId, startTimeId));
	checkResponseWithoutBody(response, 200);
}

TEST_F(RestViewTest, deleteStartTimeNotFound) {
	Response response = executeRequest("DELETE", createStartTimeUrl(IdType(), IdType()));
	checkErrorResponse(response, 404, "application/xml");
}
