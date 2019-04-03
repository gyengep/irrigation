#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockPeriodicScheduler.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


std::string RestViewTest::createPeriodicSchedulerUrl(IdType programId) {
	return createUrl("/programs/" + to_string(programId) + "/schedulers/periodic");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postPeriodicScheduler) {
	const IdType programId;

	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("POST", createPeriodicSchedulerUrl(programId), XmlWriter().save(PeriodicSchedulerSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::testGetPeriodicScheduler(const PeriodicSchedulerSample& periodicSchedulerSample) {
	const IdType programId;
	const shared_ptr<Program> program = Program::Builder().setPeriodicScheduler(periodicSchedulerSample.getObject()).build();

	document->getPrograms().insert(programId, program);

	const Response response = executeRequest("GET", createPeriodicSchedulerUrl(programId));
	checkResponseWithBody(response, 200, "application/xml");

	EXPECT_THAT(response.writeCallbackData.text, Eq(XmlWriter().save(periodicSchedulerSample.getDto())));
}

TEST_F(RestViewTest, getPeriodicScheduler1) {
	testGetPeriodicScheduler(PeriodicSchedulerSample1());
}

TEST_F(RestViewTest, getPeriodicScheduler2) {
	testGetPeriodicScheduler(PeriodicSchedulerSample2());
}

TEST_F(RestViewTest, getPeriodicScheduler3) {
	testGetPeriodicScheduler(PeriodicSchedulerSample3());
}

TEST_F(RestViewTest, getPeriodicScheduler4) {
	testGetPeriodicScheduler(PeriodicSchedulerSample4());
}

TEST_F(RestViewTest, getPeriodicSchedulerNotFound) {
	const Response response = executeRequest("GET", createPeriodicSchedulerUrl(IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getPeriodicSchedulerAcceptable) {
	const IdType programId;
	document->getPrograms().insert(programId, shared_ptr<Program>(new Program));

	const Response response = executeRequest("GET", createPeriodicSchedulerUrl(programId), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getPeriodicSchedulerNotAcceptable) {
	const IdType programId;
	document->getPrograms().insert(programId, shared_ptr<Program>(new Program));

	const Response response = executeRequest("GET", createPeriodicSchedulerUrl(programId), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchPeriodicScheduler) {
	const IdType programId;
	const shared_ptr<MockPeriodicScheduler> mockPeriodicScheduler(new MockPeriodicScheduler());
	const shared_ptr<Program> program = Program::Builder().setPeriodicScheduler(mockPeriodicScheduler).build();

	const PeriodicSchedulerSample1 periodicSchedulerSample;

	EXPECT_CALL(*mockPeriodicScheduler, updateFromPeriodicSchedulerDto(periodicSchedulerSample.getDto()));

	document->getPrograms().insert(programId, program);

	Response response = executeRequest("PATCH", createPeriodicSchedulerUrl(programId), XmlWriter().save(periodicSchedulerSample.getDto()), "application/xml");
	checkResponseWithoutBody(response, 204);
}

TEST_F(RestViewTest, patchPeriodicSchedulerNotFound) {
	Response response = executeRequest("PATCH", createPeriodicSchedulerUrl(IdType()), XmlWriter().save(ProgramSample1().getDto()), "application/xml");
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, patchPeriodicSchedulerInvalidXml) {
	const IdType programId;

	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("PATCH", createPeriodicSchedulerUrl(programId), "InvalidXml", "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchPeriodicSchedulerInvalidContentType) {
	const IdType programId;

	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("PATCH", createPeriodicSchedulerUrl(programId), "{ \"key\" = \"value\" }", "application/json");
	checkErrorResponse(response, 415, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, deletePeriodicScheduler) {
	const IdType programId;

	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("DELETE", createPeriodicSchedulerUrl(programId));
	checkErrorResponse(response, 405, "application/xml");
}
