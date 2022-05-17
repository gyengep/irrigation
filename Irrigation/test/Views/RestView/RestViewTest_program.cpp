#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockProgramContainer.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


string RestViewTest::createProgramUrl(IdType programId) {
	return createUrl("/programs/" + programId.toString());
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postProgram) {
	Response response = executeRequest("POST", createProgramUrl(IdType()), XmlWriter().save(ProgramSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

void RestViewTest::testGetProgram(const ProgramListSample& programListSample) {
	irrigationDocument = IrrigationDocument::Builder().setProgramContainer(programListSample.getContainerPtr()).build();
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager,
			"/tmp"
		)));

	const std::string piName = "xml-stylesheet";
	const std::string piValue = "type=\"text/xsl\" href=\"/program.xsl\"";

	for (const auto& programWithId : *programListSample.getContainerPtr()) {
		const Response response = executeRequest("GET", createProgramUrl(programWithId.first));
		checkResponseWithBody(response, 200, "application/xml");

		EXPECT_THAT(response.curlStringWriter.getText(), Eq(XmlWriter().save(programWithId.second->toProgramDto(), piName, piValue)));
		EXPECT_FALSE(irrigationDocument->isModified());
	}
}

TEST_F(RestViewTest, getProgram1) {
	testGetProgram(ProgramListSample1());
}

TEST_F(RestViewTest, getProgram2) {
	testGetProgram(ProgramListSample2());
}

TEST_F(RestViewTest, getProgram3) {
	testGetProgram(ProgramListSample3());
}

TEST_F(RestViewTest, getProgram4) {
	testGetProgram(ProgramListSample4());
}

TEST_F(RestViewTest, getProgramNotFound) {
	Response response = executeRequest("GET", createProgramUrl(IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getProgramAcceptable) {
	const IdType programId;
	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("GET", createProgramUrl(programId), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getProgramNotAcceptable) {
	const IdType programId;
	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("GET", createProgramUrl(programId), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchProgram) {
	const IdType programId;
	const ProgramSample4 programSample;
	const shared_ptr<MockProgram> program(new MockProgram());

	EXPECT_CALL(*program, updateFromProgramDto(programSample.getDto()));

	irrigationDocument->getPrograms().insert(programId, program);

	Response response = executeRequest("PATCH", createProgramUrl(programId), XmlWriter().save(programSample.getDto()), "application/xml");
	checkResponseWithoutBody(response, 204);
	EXPECT_TRUE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, patchProgramNotFound) {
	Response response = executeRequest("PATCH", createProgramUrl(IdType()), XmlWriter().save(ProgramSample1().getDto()), "application/xml");
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, patchProgramInvalidXml) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("PATCH", createProgramUrl(programId), "InvalidXml", "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchProgramInvalidContentType) {
	const IdType programId;

	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("PATCH", createProgramUrl(programId), "{ \"key\" = \"value\" }", "application/json");
	checkErrorResponse(response, 415, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, deleteProgram) {
	const IdType programId;
	const shared_ptr<MockProgramContainer> programContainer(new MockProgramContainer());

	EXPECT_CALL(*programContainer, erase(programId));
	EXPECT_CALL(*programContainer, insert(_, _)).Times(AnyNumber());

	irrigationDocument = IrrigationDocument::Builder().setProgramContainer(programContainer).build();
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager,
			"/tmp"
		)));
	irrigationDocument->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("DELETE", createProgramUrl(programId));
	checkResponseWithoutBody(response, 200);
	EXPECT_THAT(response.curlStringWriter.getText(), IsEmpty());
	EXPECT_TRUE(irrigationDocument->isModified());
}

TEST_F(RestViewTest, deleteProgramNotFound) {
	Response response = executeRequest("DELETE", createProgramUrl(IdType()));
	checkErrorResponse(response, 404, "application/xml");
}
