#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockStartTimeContainer.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


std::string RestViewTest::createStartTimeListUrl(IdType programId) {
	return createUrl("/programs/" + to_string(programId) + "/starttimes");
}

void RestViewTest::testGetStartTimeList(const IdType& programId, const list<StartTimeDTO>& startTimeDtoList) {
	Response response = executeRequest("GET", createStartTimeListUrl(programId));
	EXPECT_THAT(response.responseCode, Eq(200));
	EXPECT_THAT(response.writeCallbackData.text, Eq(XmlWriter().save(startTimeDtoList)));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postStartTimeList) {
	shared_ptr<MockStartTimeContainer> mockStartTimeContainer(new MockStartTimeContainer());

	EXPECT_CALL(*mockStartTimeContainer, insert(_, Pointee(*StartTimeSample1().getObject())));

	const IdType programId;
	const shared_ptr<Program> program = Program::Builder().setStartTimeContainer(mockStartTimeContainer).build();
	document->getPrograms().insert(programId, program);

	const Response response = executeRequest("POST", createStartTimeListUrl(programId), XmlWriter().save(StartTimeSample1().getDto()));
	const IdType startTimeId = document->getPrograms().at(programId)->getStartTimes().begin()->first;

	EXPECT_THAT(response.responseCode, Eq(201));
	EXPECT_THAT(response.headerCallbackData.headers,
			Contains("Location: /programs/" + to_string(programId) + "/starttimes/" + to_string(startTimeId) + "\r\n"));
}

TEST_F(RestViewTest, postStartTimeListInvalidXml) {
	const IdType programId;
	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("POST", createStartTimeListUrl(programId), "InvalidXml");
	EXPECT_THAT(response.responseCode, Eq(400));
}

TEST_F(RestViewTest, postStartTimeListNotFound) {
	const Response response = executeRequest("POST", createStartTimeListUrl(IdType()), XmlWriter().save(StartTimeSample1().getDto()));
	EXPECT_THAT(response.responseCode, Eq(404));
}

TEST_F(RestViewTest, getStartTimeList1) {
	const IdType programId;
	const shared_ptr<Program> program(new Program());
	const list<StartTimeDTO> startTimeDtoList = StartTimeListSample1().getDtoList();

	document->getPrograms().insert(programId, program);
	program->getStartTimes().updateFromStartTimeDtoList(startTimeDtoList);
	testGetStartTimeList(programId, startTimeDtoList);
}

TEST_F(RestViewTest, getStartTimeList2) {
	const IdType programId;
	const shared_ptr<Program> program(new Program());
	const list<StartTimeDTO> startTimeDtoList = StartTimeListSample2().getDtoList();

	document->getPrograms().insert(programId, program);
	program->getStartTimes().updateFromStartTimeDtoList(startTimeDtoList);
	testGetStartTimeList(programId, startTimeDtoList);
}

TEST_F(RestViewTest, getStartTimeList3) {
	const IdType programId;
	const shared_ptr<Program> program(new Program());
	const list<StartTimeDTO> startTimeDtoList = StartTimeListSample3().getDtoList();

	document->getPrograms().insert(programId, program);
	program->getStartTimes().updateFromStartTimeDtoList(startTimeDtoList);
	testGetStartTimeList(programId, startTimeDtoList);
}

TEST_F(RestViewTest, getStartTimeList4) {
	const IdType programId;
	const shared_ptr<Program> program(new Program());
	const list<StartTimeDTO> startTimeDtoList = StartTimeListSample4().getDtoList();

	document->getPrograms().insert(programId, program);
	program->getStartTimes().updateFromStartTimeDtoList(startTimeDtoList);
	testGetStartTimeList(programId, startTimeDtoList);
}

TEST_F(RestViewTest, getStartTimeListNotFound) {
	Response response = executeRequest("GET", createStartTimeListUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(404));
}

TEST_F(RestViewTest, putStartTimeList) {
	Response response = executeRequest("PUT", createStartTimeListUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(405));
}

TEST_F(RestViewTest, patchStartTimeList) {
	Response response = executeRequest("PATCH", createStartTimeListUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(405));
}

TEST_F(RestViewTest, deleteStartTimeList) {
	Response response = executeRequest("DELETE", createStartTimeListUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(405));
}
