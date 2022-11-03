#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockWeeklyScheduler.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


string RestViewTest::createWeeklySchedulerUrl(IdType programId) {
	return createUrl("/programs/" + programId.toString() + "/schedulers/weekly");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postWeeklyScheduler) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, ProgramImpl::Builder().build());

	Response response = executeRequest("POST", createWeeklySchedulerUrl(programId), XmlWriter().save(WeeklySchedulerSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::testGetWeeklyScheduler(const WeeklySchedulerSample& weeklySchedulerSample) {
	const IdType programId;
	const shared_ptr<Program> program =
		ProgramImpl::Builder()
			.setSchedulerContainer(
				SchedulerContainerImpl::Builder()
					.setWeeklyScheduler(weeklySchedulerSample.getObjectPtr())
					.build()
			)
			.build();

	irrigationDocument->getPrograms().insert(programId, program);

	const Response response = executeRequest("GET", createWeeklySchedulerUrl(programId));
	checkResponseWithBody(response, 200, "application/xml");

	EXPECT_THAT(response.curlStringWriter.getText(), Eq(XmlWriter().save(weeklySchedulerSample.getDto())));
	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, getWeeklyScheduler1) {
	testGetWeeklyScheduler(WeeklySchedulerSample1());
}

TEST_F(RestViewTest, getWeeklyScheduler2) {
	testGetWeeklyScheduler(WeeklySchedulerSample2());
}

TEST_F(RestViewTest, getWeeklyScheduler3) {
	testGetWeeklyScheduler(WeeklySchedulerSample3());
}

TEST_F(RestViewTest, getWeeklyScheduler4) {
	testGetWeeklyScheduler(WeeklySchedulerSample4());
}

TEST_F(RestViewTest, getWeeklySchedulerNotFound) {
	const Response response = executeRequest("GET", createWeeklySchedulerUrl(IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getWeeklySchedulerAcceptable) {
	const IdType programId;
	irrigationDocument->getPrograms().insert(programId, ProgramImpl::Builder().build());

	const Response response = executeRequest("GET", createWeeklySchedulerUrl(programId), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getWeeklySchedulerNotAcceptable) {
	const IdType programId;
	irrigationDocument->getPrograms().insert(programId, ProgramImpl::Builder().build());

	const Response response = executeRequest("GET", createWeeklySchedulerUrl(programId), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchWeeklyScheduler) {
	const IdType programId;
	const shared_ptr<MockWeeklyScheduler> mockWeeklyScheduler(new MockWeeklyScheduler());
	const shared_ptr<Program> program =
		ProgramImpl::Builder()
			.setSchedulerContainer(
				SchedulerContainerImpl::Builder()
					.setWeeklyScheduler(mockWeeklyScheduler)
					.build()
			)
			.build();

	const WeeklySchedulerSample1 weeklySchedulerSample;

	EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(weeklySchedulerSample.getDto()));

	irrigationDocument->getPrograms().insert(programId, program);

	Response response = executeRequest("PATCH", createWeeklySchedulerUrl(programId), XmlWriter().save(weeklySchedulerSample.getDto()), "application/xml");
	checkResponseWithoutBody(response, 204);
	EXPECT_TRUE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, patchWeeklySchedulerNotFound) {
	Response response = executeRequest("PATCH", createWeeklySchedulerUrl(IdType()), XmlWriter().save(WeeklySchedulerSample1().getDto()), "application/xml");
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, patchWeeklySchedulerInvalidXml) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, ProgramImpl::Builder().build());

	const Response response = executeRequest("PATCH", createWeeklySchedulerUrl(programId), "InvalidXml", "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchWeeklySchedulerInvalidContentType) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, ProgramImpl::Builder().build());

	const Response response = executeRequest("PATCH", createWeeklySchedulerUrl(programId), "{ \"key\" = \"value\" }", "application/json");
	checkErrorResponse(response, 415, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, deleteWeeklyScheduler) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, ProgramImpl::Builder().build());

	Response response = executeRequest("DELETE", createWeeklySchedulerUrl(programId));
	checkErrorResponse(response, 405, "application/xml");
}
