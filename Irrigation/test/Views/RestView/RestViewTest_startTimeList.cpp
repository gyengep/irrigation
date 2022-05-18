#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockStartTimeContainer.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


string RestViewTest::createStartTimeListUrl(IdType programId) {
	return createUrl("/programs/" + programId.toString() + "/starttimes");
}

///////////////////////////////////////////////////////////////////////////////
/*
TEST_F(RestViewTest, postStartTimeList) {
	shared_ptr<MockStartTimeContainer> mockStartTimeContainer = std::make_shared<MockStartTimeContainer>();

	EXPECT_CALL(*mockStartTimeContainer, insert(_, Pointee(Eq(std::ref(*StartTimeSample1().getObjectPtr())))));

	const IdType programId;
	const shared_ptr<Program> program = Program::Builder().setStartTimeContainer(mockStartTimeContainer).build();

	irrigationDocument->getPrograms().insert(programId, program);

	const Response response = executeRequest("POST", createStartTimeListUrl(programId), XmlWriter().save(StartTimeSample1().getDto()), "application/xml");
	const IdType startTimeId = irrigationDocument->getPrograms().at(programId)->getStartTimes().begin()->first;

	checkResponseWithoutBody(response, 201);
	EXPECT_THAT(response.curlHeaderWriter.getHeaders(), Contains("Location: /programs/" + programId.toString() + "/starttimes/" + startTimeId.toString() + "\r\n"));
	EXPECT_TRUE(irrigationDocument->isModified());
}
*/
TEST_F(RestViewTest, postStartTimeListInvalidXml) {
	const IdType programId;
	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("POST", createStartTimeListUrl(programId), "InvalidXml", "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, postStartTimeListInvalidContentType) {
	const IdType programId;
	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("POST", createStartTimeListUrl(programId), "InvalidXml", "application/json");
	checkErrorResponse(response, 415, "application/xml");
}

TEST_F(RestViewTest, postStartTimeListNotFound) {
	const Response response = executeRequest("POST", createStartTimeListUrl(IdType()), XmlWriter().save(StartTimeSample1().getDto()), "application/xml");
	checkErrorResponse(response, 404, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::testGetStartTimeList(const Dto2ObjectTest::StartTimeListSample& startTimeListSample) {
	const IdType programId;
	const shared_ptr<Program> program = Program::Builder().setStartTimeContainer(startTimeListSample.getContainerPtr()).build();

	irrigationDocument->getPrograms().insert(programId, program);

	const Response response = executeRequest("GET", createStartTimeListUrl(programId));
	checkResponseWithBody(response, 200, "application/xml");

	EXPECT_THAT(response.curlStringWriter.getText(), Eq(XmlWriter().save(startTimeListSample.getDtoList())));
	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, getStartTimeList1) {
	testGetStartTimeList(StartTimeListSample1());
}

TEST_F(RestViewTest, getStartTimeList2) {
	testGetStartTimeList(StartTimeListSample2());
}

TEST_F(RestViewTest, getStartTimeList3) {
	testGetStartTimeList(StartTimeListSample3());
}

TEST_F(RestViewTest, getStartTimeList4) {
	testGetStartTimeList(StartTimeListSample4());
}


TEST_F(RestViewTest, getStartTimeListAcceptable) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("GET", createStartTimeListUrl(programId), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getStartTimeListNotFound) {
	Response response = executeRequest("GET", createStartTimeListUrl(IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getStartTimeListNotAcceptable) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("GET", createStartTimeListUrl(programId), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchStartTimeList) {
	Response response = executeRequest("PATCH", createStartTimeListUrl(IdType()), XmlWriter().save(StartTimeSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, deleteStartTimeList) {
	Response response = executeRequest("DELETE", createStartTimeListUrl(IdType()));
	checkErrorResponse(response, 405, "application/xml");
}
