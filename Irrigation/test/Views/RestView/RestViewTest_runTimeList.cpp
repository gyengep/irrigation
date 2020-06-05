#include "RestViewTest.h"
#include "Mocks/MockRunTimeContainer.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


string RestViewTest::createRunTimeListUrl(IdType programId) {
	return createUrl("/programs/" + to_string(programId) + "/runtimes");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postRunTimeList) {
	Response response = executeRequest("POST", createRunTimeListUrl(IdType()), XmlWriter().save(RunTimeSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::testGetRunTimeList(const RunTimeListSample& runTimeListSample) {
	const IdType programId;
	const shared_ptr<Program> program = Program::Builder().setRunTimeContainer(runTimeListSample.getContainer()).build();

	irrigationDocument->getPrograms().insert(programId, program);

	const Response response = executeRequest("GET", createRunTimeListUrl(programId));
	checkResponseWithBody(response, 200, "application/xml");

	EXPECT_THAT(response.curlStringWriter.getText(), Eq(XmlWriter().save(runTimeListSample.getDtoList())));
	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, getRunTimeList1) {
	testGetRunTimeList(RunTimeListSample1());
}

TEST_F(RestViewTest, getRunTimeList2) {
	testGetRunTimeList(RunTimeListSample2());
}

TEST_F(RestViewTest, getRunTimeList3) {
	testGetRunTimeList(RunTimeListSample3());
}

TEST_F(RestViewTest, getRunTimeList4) {
	testGetRunTimeList(RunTimeListSample4());
}

TEST_F(RestViewTest, getRunTimeListAcceptable) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));
	Response response = executeRequest("GET", createRunTimeListUrl(programId), "Accept: application/xml");

	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getRunTimeListNotFound) {
	Response response = executeRequest("GET", createRunTimeListUrl(IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getRunTimeListNotAcceptable) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));
	Response response = executeRequest("GET", createRunTimeListUrl(programId), "Accept: application/json");

	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::testPatchRunTimeList(const RunTimeListSample& runTimeListSample) {
	const IdType programId;
	const shared_ptr<MockRunTimeContainer> mockRunTimeContainer(new MockRunTimeContainer());
	const shared_ptr<Program> program = Program::Builder().setRunTimeContainer(mockRunTimeContainer).build();

	irrigationDocument->getPrograms().insert(programId, program);

	EXPECT_CALL(*mockRunTimeContainer, updateFromRunTimeDtoList(runTimeListSample.getDtoList()));

	const Response response = executeRequest("PATCH", createRunTimeListUrl(programId), XmlWriter().save(runTimeListSample.getDtoList()), "application/xml");
	checkResponseWithoutBody(response, 204);
	EXPECT_TRUE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, patchRunTimeList1) {
	testPatchRunTimeList(RunTimeListSample1());
}

TEST_F(RestViewTest, patchRunTimeList2) {
	testPatchRunTimeList(RunTimeListSample2());
}

TEST_F(RestViewTest, patchRunTimeList3) {
	testPatchRunTimeList(RunTimeListSample3());
}

TEST_F(RestViewTest, patchRunTimeList4) {
	testPatchRunTimeList(RunTimeListSample4());
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, deleteRunTimeList) {
	Response response = executeRequest("DELETE", createRunTimeListUrl(IdType()));
	checkErrorResponse(response, 405, "application/xml");
}
