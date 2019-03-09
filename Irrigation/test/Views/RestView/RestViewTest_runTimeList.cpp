#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


std::string RestViewTest::createRunTimeListUrl(IdType programId) {
	return createUrl("/programs/" + to_string(programId) + "/runtimes");
}

void RestViewTest::testGetRunTimeList(const IdType& programId, const list<RunTimeDTO>& runTimeDtoList) {
	Response response = executeRequest("GET", createRunTimeListUrl(programId));
	EXPECT_THAT(response.responseCode, Eq(200));
	EXPECT_THAT(response.writeCallbackData.text, Eq(XmlWriter().save(runTimeDtoList)));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postRunTimeList) {
	Response response = executeRequest("POST", createRunTimeListUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(405));
}

TEST_F(RestViewTest, getRunTimeList1) {
	const IdType programId;
	const shared_ptr<Program> program(new Program());
	const list<RunTimeDTO> runTimeDtoList = RunTimeListSample1().getDtoList();

	document->getPrograms().insert(programId, program);
	program->getRunTimes().updateFromRunTimeDtoList(runTimeDtoList);
	testGetRunTimeList(programId, runTimeDtoList);
}

TEST_F(RestViewTest, getRunTimeList2) {
	const IdType programId;
	const shared_ptr<Program> program(new Program());
	const list<RunTimeDTO> runTimeDtoList = RunTimeListSample1().getDtoList();

	document->getPrograms().insert(programId, program);
	program->getRunTimes().updateFromRunTimeDtoList(runTimeDtoList);
	testGetRunTimeList(programId, runTimeDtoList);
}

TEST_F(RestViewTest, getRunTimeList3) {
	const IdType programId;
	const shared_ptr<Program> program(new Program());
	const list<RunTimeDTO> runTimeDtoList = RunTimeListSample1().getDtoList();

	document->getPrograms().insert(programId, program);
	program->getRunTimes().updateFromRunTimeDtoList(runTimeDtoList);
	testGetRunTimeList(programId, runTimeDtoList);
}

TEST_F(RestViewTest, getRunTimeList4) {
	const IdType programId;
	const shared_ptr<Program> program(new Program());
	const list<RunTimeDTO> runTimeDtoList = RunTimeListSample1().getDtoList();

	document->getPrograms().insert(programId, program);
	program->getRunTimes().updateFromRunTimeDtoList(runTimeDtoList);
	testGetRunTimeList(programId, runTimeDtoList);
}

TEST_F(RestViewTest, getRunTimeListNotFound) {
	Response response = executeRequest("GET", createRunTimeListUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(404));
}

TEST_F(RestViewTest, putRunTimeList) {
	Response response = executeRequest("PUT", createRunTimeListUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(405));
}

TEST_F(RestViewTest, patchRunTimeList) {
	Response response = executeRequest("PATCH", createRunTimeListUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(405));
}

TEST_F(RestViewTest, deleteRunTimeList) {
	Response response = executeRequest("DELETE", createRunTimeListUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(405));
}
