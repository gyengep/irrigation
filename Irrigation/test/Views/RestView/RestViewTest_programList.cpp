#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockProgramContainer.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


string RestViewTest::createProgramListUrl() {
	return createUrl("/programs");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postProgramList) {
	shared_ptr<MockProgramContainer> mockProgramContainer(new MockProgramContainer());

	const ProgramPtr expectedParam = ProgramSample1().getObjectPtr();
	EXPECT_CALL(*mockProgramContainer, insert(_, Pointee(Eq(std::ref(*expectedParam)))));

	irrigationDocument = IrrigationDocument::Builder().setProgramContainer(mockProgramContainer).build();
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager,
			"/tmp"
		)));

	const Response response = executeRequest("POST", createProgramListUrl(), XmlWriter().save(ProgramSample1().getDto()), "application/xml");
	const IdType programId = irrigationDocument->getPrograms().begin()->first;

	checkResponseWithoutBody(response, 201);
	EXPECT_THAT(response.curlHeaderWriter.getHeaders(), Contains("Location: /programs/" + to_string(programId) + "\r\n"));
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

void RestViewTest::testGetProgramList(const ProgramListSample& programListSample) {
	irrigationDocument = IrrigationDocument::Builder().setProgramContainer(programListSample.getContainerPtr()).build();
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager,
			"/tmp"
		)));

	const Response response = executeRequest("GET", createProgramListUrl());
	checkResponseWithBody(response, 200, "application/xml");

	const std::string piName = "xml-stylesheet";
	const std::string piValue = "type=\"text/xsl\" href=\"/programlist.xsl\"";

	EXPECT_THAT(response.curlStringWriter.getText(), Eq(XmlWriter().save(programListSample.getDtoList(), piName, piValue)));
	EXPECT_FALSE(irrigationDocument->isModified());
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
