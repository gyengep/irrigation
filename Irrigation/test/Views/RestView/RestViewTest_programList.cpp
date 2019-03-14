#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockProgramContainer.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


std::string RestViewTest::createProgramListUrl() {
	return createUrl("/programs");
}

void RestViewTest::testGetProgramList(const list<ProgramDTO>& programDtoList) {
	Response response = executeRequest("GET", createProgramListUrl());
	EXPECT_THAT(response.responseCode, Eq(200));
	EXPECT_THAT(response.writeCallbackData.text, Eq(XmlWriter().save(programDtoList)));
	EXPECT_THAT(response.headerCallbackData.headers, Contains("Content-Type: application/xml\r\n"));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postProgramList) {
	shared_ptr<MockProgramContainer> mockProgramContainer(new MockProgramContainer());

	EXPECT_CALL(*mockProgramContainer, insert(_, Pointee(*ProgramSample1().getObject())));

	document = IrrigationDocument::Builder().setProgramContainer(mockProgramContainer).build();
	document->addView(unique_ptr<View>(new RestView(*document, port)));

	const Response response = executeRequest("POST", createProgramListUrl(), XmlWriter().save(ProgramSample1().getDto()));
	const IdType programId = document->getPrograms().begin()->first;

	EXPECT_THAT(response.responseCode, Eq(201));
	EXPECT_THAT(response.headerCallbackData.headers, Contains("Location: /programs/" + to_string(programId) + "\r\n"));
	EXPECT_THAT(response.headerCallbackData.headers, Not(Contains(HasSubstr("Content-Type:"))));
}

TEST_F(RestViewTest, postProgramListInvalidXml) {
	const Response response = executeRequest("POST", createProgramListUrl(), "InvalidXml");
	EXPECT_THAT(response.responseCode, Eq(400));
	EXPECT_THAT(response.headerCallbackData.headers, Contains("Content-Type: application/xml\r\n"));
}

TEST_F(RestViewTest, getProgramList1) {
	const std::list<ProgramDTO> programDtoList = ProgramListSample1().getDtoList();
	document->getPrograms().updateFromProgramDtoList(programDtoList);
	testGetProgramList(programDtoList);
}

TEST_F(RestViewTest, getProgramList2) {
	const std::list<ProgramDTO> programDtoList = ProgramListSample2().getDtoList();
	document->getPrograms().updateFromProgramDtoList(programDtoList);
	testGetProgramList(programDtoList);
}

TEST_F(RestViewTest, getProgramList3) {
	const std::list<ProgramDTO> programDtoList = ProgramListSample3().getDtoList();
	document->getPrograms().updateFromProgramDtoList(programDtoList);
	testGetProgramList(programDtoList);
}

TEST_F(RestViewTest, getProgramList4) {
	const std::list<ProgramDTO> programDtoList = ProgramListSample4().getDtoList();
	document->getPrograms().updateFromProgramDtoList(programDtoList);
	testGetProgramList(programDtoList);
}

TEST_F(RestViewTest, putProgramList) {
	Response response = executeRequest("PUT", createProgramListUrl(), XmlWriter().save(ProgramSample1().getDto()));
	EXPECT_THAT(response.responseCode, Eq(405));
	EXPECT_THAT(response.headerCallbackData.headers, Contains("Content-Type: application/xml\r\n"));
}

TEST_F(RestViewTest, patchProgramList) {
	Response response = executeRequest("PATCH", createProgramListUrl(), XmlWriter().save(ProgramSample1().getDto()));
	EXPECT_THAT(response.responseCode, Eq(405));
	EXPECT_THAT(response.headerCallbackData.headers, Contains("Content-Type: application/xml\r\n"));
}

TEST_F(RestViewTest, deleteProgramList) {
	Response response = executeRequest("DELETE", createProgramListUrl());
	EXPECT_THAT(response.responseCode, Eq(405));
	EXPECT_THAT(response.headerCallbackData.headers, Contains("Content-Type: application/xml\r\n"));
}
