#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockTemperatureDependentScheduler.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


string RestViewTest::createTemperatureDependentSchedulerUrl(IdType programId) {
	return createUrl("/programs/" + to_string(programId) + "/schedulers/temperature-dependent");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postTemperatureDependentScheduler) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("POST", createTemperatureDependentSchedulerUrl(programId), XmlWriter().save(TemperatureDependentSchedulerSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::testGetTemperatureDependentScheduler(const TemperatureDependentSchedulerSample& temperatureDependentSchedulerSample) {
	const IdType programId;
	const shared_ptr<Program> program = Program::Builder().setTemperatureDependentScheduler(temperatureDependentSchedulerSample.getObject()).build();

	irrigationDocument->getPrograms().insert(programId, program);

	const Response response = executeRequest("GET", createTemperatureDependentSchedulerUrl(programId));
	checkResponseWithBody(response, 200, "application/xml");

	EXPECT_THAT(response.curlStringWriter.getText(), Eq(XmlWriter().save(temperatureDependentSchedulerSample.getDto())));
	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, getTemperatureDependentScheduler1) {
	testGetTemperatureDependentScheduler(TemperatureDependentSchedulerSample1());
}

TEST_F(RestViewTest, getTemperatureDependentScheduler2) {
	testGetTemperatureDependentScheduler(TemperatureDependentSchedulerSample2());
}

TEST_F(RestViewTest, getTemperatureDependentScheduler3) {
	testGetTemperatureDependentScheduler(TemperatureDependentSchedulerSample3());
}

TEST_F(RestViewTest, getTemperatureDependentScheduler4) {
	testGetTemperatureDependentScheduler(TemperatureDependentSchedulerSample4());
}

TEST_F(RestViewTest, getTemperatureDependentSchedulerNotFound) {
	const Response response = executeRequest("GET", createTemperatureDependentSchedulerUrl(IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getTemperatureDependentSchedulerAcceptable) {
	const IdType programId;
	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program));

	const Response response = executeRequest("GET", createTemperatureDependentSchedulerUrl(programId), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getTemperatureDependentSchedulerNotAcceptable) {
	const IdType programId;
	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program));

	const Response response = executeRequest("GET", createTemperatureDependentSchedulerUrl(programId), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchTemperatureDependentScheduler) {
	const IdType programId;
	const shared_ptr<MockTemperatureDependentScheduler> mockTemperatureDependentScheduler(new MockTemperatureDependentScheduler());
	const shared_ptr<Program> program = Program::Builder().setTemperatureDependentScheduler(mockTemperatureDependentScheduler).build();

	const TemperatureDependentSchedulerSample1 temperatureDependentSchedulerSample;

	EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(temperatureDependentSchedulerSample.getDto()));

	irrigationDocument->getPrograms().insert(programId, program);

	Response response = executeRequest("PATCH", createTemperatureDependentSchedulerUrl(programId), XmlWriter().save(temperatureDependentSchedulerSample.getDto()), "application/xml");
	checkResponseWithoutBody(response, 204);
	EXPECT_TRUE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, patchTemperatureDependentSchedulerNotFound) {
	Response response = executeRequest("PATCH", createTemperatureDependentSchedulerUrl(IdType()), XmlWriter().save(TemperatureDependentSchedulerSample2().getDto()), "application/xml");
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, patchTemperatureDependentSchedulerInvalidXml) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("PATCH", createTemperatureDependentSchedulerUrl(programId), "InvalidXml", "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchTemperatureDependentSchedulerInvalidContentType) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("PATCH", createTemperatureDependentSchedulerUrl(programId), "{ \"key\" = \"value\" }", "application/json");
	checkErrorResponse(response, 415, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, deleteTemperatureDependentScheduler) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("DELETE", createTemperatureDependentSchedulerUrl(programId));
	checkErrorResponse(response, 405, "application/xml");
}
