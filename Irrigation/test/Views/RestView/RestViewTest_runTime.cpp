#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockRunTime.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


std::string RestViewTest::createRunTimeUrl(IdType programId, IdType runTimeId) {
	return createUrl("/programs/" + to_string(programId) + "/runtimes/" + to_string(runTimeId));
}

void RestViewTest::testGetRunTime(const IdType& programId, const IdType& runTimeId, const RunTimeDTO& runTimeDTO) {
	Response response = executeRequest("GET", createRunTimeUrl(programId, runTimeId));
	checkResponseWithBody(response, 200, "application/xml");
	EXPECT_THAT(response.writeCallbackData.text, Eq(XmlWriter().save(runTimeDTO)));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postRunTime) {
	Response response = executeRequest("POST", createRunTimeUrl(IdType(), IdType()), XmlWriter().save(RunTimeSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, getRunTime) {
	const IdType programId;
	const shared_ptr<Program> program(new Program());

	program->getRunTimes().at(0)->updateFromRunTimeDto(RunTimeSample1().getDto());
	program->getRunTimes().at(1)->updateFromRunTimeDto(RunTimeSample2().getDto());
	program->getRunTimes().at(2)->updateFromRunTimeDto(RunTimeSample3().getDto());
	program->getRunTimes().at(3)->updateFromRunTimeDto(RunTimeSample4().getDto());
	program->getRunTimes().at(4)->updateFromRunTimeDto(RunTimeSample5().getDto());
	program->getRunTimes().at(5)->updateFromRunTimeDto(RunTimeSample6().getDto());

	document->getPrograms().insert(programId, program);

	testGetRunTime(programId, 0, RunTimeSample1().getDto());
	testGetRunTime(programId, 1, RunTimeSample2().getDto());
	testGetRunTime(programId, 2, RunTimeSample3().getDto());
	testGetRunTime(programId, 3, RunTimeSample4().getDto());
	testGetRunTime(programId, 4, RunTimeSample5().getDto());
	testGetRunTime(programId, 5, RunTimeSample6().getDto());
}

TEST_F(RestViewTest, getRunTimeNotFound1) {
	Response response = executeRequest("GET", createRunTimeUrl(IdType(), IdType(0)));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getRunTimeNotFound2) {
	IdType programId;
	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("GET", createRunTimeUrl(programId, IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getRunTimeAcceptable) {
	const IdType programId;
	const IdType runTimeId(0);

	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("GET", createRunTimeUrl(programId, runTimeId), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getRunTimeNotAcceptable) {
	const IdType programId;
	const IdType runTimeId(0);

	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("GET", createRunTimeUrl(programId, runTimeId), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchRunTime) {
	const IdType programId;
	const IdType runTimeId(2);
	const RunTimeSample4 runTimeSample;
	const shared_ptr<MockRunTimeFactory> mockRunTimeFactory(new MockRunTimeFactory());
	const shared_ptr<RunTimeContainer> runTimeContainer(new RunTimeContainer(mockRunTimeFactory));
	const shared_ptr<Program> program = Program::Builder().setRunTimeContainer(runTimeContainer).build();

	document->getPrograms().insert(programId, program);

	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes.at(runTimeId), updateFromRunTimeDto(runTimeSample.getDto()));

	const Response response = executeRequest("PATCH", createRunTimeUrl(programId, runTimeId), XmlWriter().save(runTimeSample.getDto()), "application/xml");
	checkResponseWithoutBody(response, 204);
}

TEST_F(RestViewTest, patchRunTimeNotFound) {
	const Response response = executeRequest("PATCH", createRunTimeUrl(IdType(), IdType()), XmlWriter().save(RunTimeSample1().getDto()), "application/xml");
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, patchRunTimeInvalidXml) {
	const IdType programId;
	const IdType runTimeId(2);

	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("PATCH", createRunTimeUrl(programId, runTimeId), "InvalidXml", "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchRunTimeInvalidContentType) {
	const IdType programId;
	const IdType runTimeId(2);

	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("PATCH", createRunTimeUrl(programId, runTimeId), "{ \"key\" = \"value\" }", "application/json");
	checkErrorResponse(response, 415, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, deleteRunTime) {
	Response response = executeRequest("DELETE", createRunTimeUrl(IdType(), IdType()));
	checkErrorResponse(response, 405, "application/xml");
}
