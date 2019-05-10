#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockProgramContainer.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


std::string RestViewTest::createProgramListUrl(const std::string& requestParameters) {
	if (requestParameters.empty()) {
		return createUrl("/programs");
	} else {
		return createUrl("/programs") + "?" + requestParameters;
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postProgramList) {
	shared_ptr<MockProgramContainer> mockProgramContainer(new MockProgramContainer());

	EXPECT_CALL(*mockProgramContainer, insert(_, Pointee(*ProgramSample1().getObject())));

	irrigationDocument = IrrigationDocument::Builder().setProgramContainer(mockProgramContainer).build();
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port)));

	const Response response = executeRequest("POST", createProgramListUrl(), XmlWriter().save(ProgramSample1().getDto()), "application/xml");
	const IdType programId = irrigationDocument->getPrograms().begin()->first;

	checkResponseWithoutBody(response, 201);
	EXPECT_THAT(response.headerCallbackData.headers, Contains("Location: /programs/" + to_string(programId) + "\r\n"));
	EXPECT_TRUE(irrigationDocument->isModified());
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

void RestViewTest::testGetProgramList(const ProgramListSample& programListSample, const std::string& requestParameters, bool includeContainers) {
	irrigationDocument = IrrigationDocument::Builder().setProgramContainer(programListSample.getContainer()).build();
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port)));

	const Response response = executeRequest("GET", createProgramListUrl(requestParameters));
	checkResponseWithBody(response, 200, "application/xml");

	EXPECT_THAT(response.writeCallbackData.text, Eq(XmlWriter().save(programListSample.getDtoList(), includeContainers)));
	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, getProgramList1) {
	testGetProgramList(ProgramListSample1(), "", false);
}

TEST_F(RestViewTest, getProgramList2) {
	testGetProgramList(ProgramListSample2(), "", false);
}

TEST_F(RestViewTest, getProgramList3) {
	testGetProgramList(ProgramListSample2(), "", false);
}

TEST_F(RestViewTest, getProgramList4) {
	testGetProgramList(ProgramListSample4(), "", false);
}

TEST_F(RestViewTest, getProgramListIncludeContainers1) {
	testGetProgramList(ProgramListSample1(), "include-containers=true", true);
}

TEST_F(RestViewTest, getProgramListIncludeContainers2) {
	testGetProgramList(ProgramListSample2(), "include-containers=true", true);
}

TEST_F(RestViewTest, getProgramListIncludeContainers3) {
	testGetProgramList(ProgramListSample2(), "include-containers=true", true);
}

TEST_F(RestViewTest, getProgramListIncludeContainers4) {
	testGetProgramList(ProgramListSample4(), "include-containers=true", true);
}

TEST_F(RestViewTest, getProgramListNotIncludeContainers1) {
	testGetProgramList(ProgramListSample1(), "include-containers=false", false);
}

TEST_F(RestViewTest, getProgramListNotIncludeContainers2) {
	testGetProgramList(ProgramListSample2(), "include-containers=false", false);
}

TEST_F(RestViewTest, getProgramListNotIncludeContainers3) {
	testGetProgramList(ProgramListSample2(), "include-containers=false", false);
}

TEST_F(RestViewTest, getProgramListNotIncludeContainers4) {
	testGetProgramList(ProgramListSample4(), "include-containers=false", false);
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
