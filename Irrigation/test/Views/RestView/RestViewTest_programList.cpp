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

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postProgramList) {
	shared_ptr<MockProgramContainer> mockProgramContainer(new MockProgramContainer());

	EXPECT_CALL(*mockProgramContainer, insert(_, Pointee(*ProgramSample1().getObject())));

	document = IrrigationDocument::Builder().setProgramContainer(mockProgramContainer).build();
	document->addView(unique_ptr<View>(new RestView(*document, port)));

	const Response response = executeRequest("POST", createProgramListUrl(), XmlWriter().save(ProgramSample1().getDto()), "application/xml");
	const IdType programId = document->getPrograms().begin()->first;

	checkResponseWithoutBody(response, 201);
	EXPECT_THAT(response.headerCallbackData.headers, Contains("Location: /programs/" + to_string(programId) + "\r\n"));
}

TEST_F(RestViewTest, postProgramListInvalidXml) {
	const Response response = executeRequest("POST", createProgramListUrl(), "InvalidXml", "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, postProgramListInvalidContentType) {
	const Response response = executeRequest("POST", createProgramListUrl(), "{ \"key\" : \"value\" }", "application/json");
	checkErrorResponse(response, 415, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::testGetProgramList(const ProgramListSample& programListSample) {
	document = IrrigationDocument::Builder().setProgramContainer(programListSample.getContainer()).build();
	document->addView(unique_ptr<View>(new RestView(*document, port)));

	const Response response = executeRequest("GET", createProgramListUrl());
	checkResponseWithBody(response, 200, "application/xml");

	EXPECT_THAT(response.writeCallbackData.text, Eq(XmlWriter().save(programListSample.getDtoList(), false)));
}

TEST_F(RestViewTest, getProgramList1) {
	testGetProgramList(ProgramListSample1());
}

TEST_F(RestViewTest, getProgramList2) {
	testGetProgramList(ProgramListSample2());
}

TEST_F(RestViewTest, getProgramList3) {
	testGetProgramList(ProgramListSample2());
}

TEST_F(RestViewTest, getProgramList4) {
	testGetProgramList(ProgramListSample4());
}

TEST_F(RestViewTest, getProgramListAcceptable) {
	Response response = executeRequest("GET", createProgramListUrl(), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getProgramListNotAcceptable) {
	Response response = executeRequest("GET", createProgramListUrl(), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchProgramList) {
	Response response = executeRequest("PATCH", createProgramListUrl(), XmlWriter().save(ProgramSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, deleteProgramList) {
	Response response = executeRequest("DELETE", createProgramListUrl());
	checkErrorResponse(response, 405, "application/xml");
}
