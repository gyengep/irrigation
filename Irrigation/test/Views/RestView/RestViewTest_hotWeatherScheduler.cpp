#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockHotWeatherScheduler.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


string RestViewTest::createHotWeatherSchedulerUrl(IdType programId) {
	return createUrl("/programs/" + programId.toString() + "/schedulers/hot-weather");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postHotWeatherScheduler) {
	const IdType programId;

	irrigationDocument->getProgramContainer().insert(programId, ProgramImpl::Builder().build());

	Response response = executeRequest("POST", createHotWeatherSchedulerUrl(programId), XmlWriter().save(HotWeatherSchedulerSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::testGetHotWeatherScheduler(const HotWeatherSchedulerSample& hotWeatherSchedulerSample) {
	const IdType programId;
	const shared_ptr<Program> program =
		ProgramImpl::Builder()
			.setSchedulerContainer(
				SchedulerContainerImpl::Builder()
					.setHotWeatherScheduler(hotWeatherSchedulerSample.getObjectPtr())
					.build()
			)
			.build();

	irrigationDocument->getProgramContainer().insert(programId, program);

	const Response response = executeRequest("GET", createHotWeatherSchedulerUrl(programId));
	checkResponseWithBody(response, 200, "application/xml");

	EXPECT_THAT(response.curlStringWriter.getText(), Eq(XmlWriter().save(hotWeatherSchedulerSample.getDto())));
	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, getHotWeatherScheduler1) {
	testGetHotWeatherScheduler(HotWeatherSchedulerSample1());
}

TEST_F(RestViewTest, getHotWeatherScheduler2) {
	testGetHotWeatherScheduler(HotWeatherSchedulerSample2());
}

TEST_F(RestViewTest, getHotWeatherScheduler3) {
	testGetHotWeatherScheduler(HotWeatherSchedulerSample3());
}

TEST_F(RestViewTest, getHotWeatherScheduler4) {
	testGetHotWeatherScheduler(HotWeatherSchedulerSample4());
}

TEST_F(RestViewTest, getHotWeatherSchedulerNotFound) {
	const Response response = executeRequest("GET", createHotWeatherSchedulerUrl(IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getHotWeatherSchedulerAcceptable) {
	const IdType programId;
	irrigationDocument->getProgramContainer().insert(programId, ProgramImpl::Builder().build());

	const Response response = executeRequest("GET", createHotWeatherSchedulerUrl(programId), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getHotWeatherSchedulerNotAcceptable) {
	const IdType programId;
	irrigationDocument->getProgramContainer().insert(programId, ProgramImpl::Builder().build());

	const Response response = executeRequest("GET", createHotWeatherSchedulerUrl(programId), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchHotWeatherScheduler) {
	const IdType programId;
	const shared_ptr<MockHotWeatherScheduler> mockHotWeatherScheduler(new MockHotWeatherScheduler());
	const shared_ptr<Program> program =
		ProgramImpl::Builder()
			.setSchedulerContainer(
				SchedulerContainerImpl::Builder()
					.setHotWeatherScheduler(mockHotWeatherScheduler)
					.build()
			)
			.build();

	const HotWeatherSchedulerSample1 hotWeatherSchedulerSample;

	EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(hotWeatherSchedulerSample.getDto()));

	irrigationDocument->getProgramContainer().insert(programId, program);

	Response response = executeRequest("PATCH", createHotWeatherSchedulerUrl(programId), XmlWriter().save(hotWeatherSchedulerSample.getDto()), "application/xml");
	checkResponseWithoutBody(response, 204);
	EXPECT_TRUE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, patchHotWeatherSchedulerNotFound) {
	Response response = executeRequest("PATCH", createHotWeatherSchedulerUrl(IdType()), XmlWriter().save(HotWeatherSchedulerSample2().getDto()), "application/xml");
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, patchHotWeatherSchedulerInvalidXml) {
	const IdType programId;

	irrigationDocument->getProgramContainer().insert(programId, ProgramImpl::Builder().build());

	const Response response = executeRequest("PATCH", createHotWeatherSchedulerUrl(programId), "InvalidXml", "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchHotWeatherSchedulerInvalidContentType) {
	const IdType programId;

	irrigationDocument->getProgramContainer().insert(programId, ProgramImpl::Builder().build());

	const Response response = executeRequest("PATCH", createHotWeatherSchedulerUrl(programId), "{ \"key\" = \"value\" }", "application/json");
	checkErrorResponse(response, 415, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, deleteHotWeatherScheduler) {
	const IdType programId;

	irrigationDocument->getProgramContainer().insert(programId, ProgramImpl::Builder().build());

	Response response = executeRequest("DELETE", createHotWeatherSchedulerUrl(programId));
	checkErrorResponse(response, 405, "application/xml");
}
