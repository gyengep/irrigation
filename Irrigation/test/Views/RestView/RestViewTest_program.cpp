#include "RestViewTest.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockProgramContainer.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


std::string RestViewTest::createProgramUrl(IdType programId) {
	return createUrl("/programs/" + to_string(programId));
}

void RestViewTest::testGetProgram(const IdType& programId, const ProgramDTO& programDto) {
	Response response = executeRequest("GET", createProgramUrl(programId));
	checkResponseWithBody(response, 200, "application/xml");
	EXPECT_THAT(response.writeCallbackData.text, Eq(XmlWriter().save(programDto)));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postProgram) {
	Response response = executeRequest("POST", createProgramUrl(IdType()), XmlWriter().save(ProgramSample1().getDto()), "application/xml");
	checkErrorResponse(response, 405, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, getProgram) {
	const IdType programId1, programId2, programId3, programId4;
	document->getPrograms().insert(programId1, ProgramSample1().getObject());
	document->getPrograms().insert(programId2, ProgramSample2().getObject());
	document->getPrograms().insert(programId3, ProgramSample3().getObject());
	document->getPrograms().insert(programId4, ProgramSample4().getObject());

	testGetProgram(programId1, ProgramSample1().getDto());
	testGetProgram(programId2, ProgramSample2().getDto());
	testGetProgram(programId3, ProgramSample3().getDto());
	testGetProgram(programId4, ProgramSample4().getDto());
}

TEST_F(RestViewTest, getProgramNotFound) {
	Response response = executeRequest("GET", createProgramUrl(IdType()));
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, getProgramAcceptable) {
	const IdType programId;
	document->getPrograms().insert(programId, ProgramSample1().getObject());

	Response response = executeRequest("GET", createProgramUrl(programId), "Accept: application/xml");
	checkResponseWithBody(response, 200, "application/xml");
}

TEST_F(RestViewTest, getProgramNotAcceptable) {
	const IdType programId;
	document->getPrograms().insert(programId, ProgramSample1().getObject());

	Response response = executeRequest("GET", createProgramUrl(programId), "Accept: application/json");
	checkErrorResponse(response, 406, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchProgram) {
	const IdType programId;
	const ProgramSample4 programSample;
	const shared_ptr<MockProgram> program(new MockProgram());

	EXPECT_CALL(*program, updateFromProgramDto(programSample.getDto()));

	document->getPrograms().insert(programId, program);

	Response response = executeRequest("PATCH", createProgramUrl(programId), XmlWriter().save(programSample.getDto()), "application/xml");
	checkResponseWithoutBody(response, 204);
}

TEST_F(RestViewTest, patchProgramNotFound) {
	Response response = executeRequest("PATCH", createProgramUrl(IdType()), XmlWriter().save(ProgramSample1().getDto()), "application/xml");
	checkErrorResponse(response, 404, "application/xml");
}

TEST_F(RestViewTest, patchProgramInvalidXml) {
	const IdType programId;

	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("PATCH", createProgramUrl(programId), "InvalidXml", "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchProgramInvalidContentType) {
	const IdType programId;

	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("PATCH", createProgramUrl(programId), "{ \"key\" = \"value\" }", "application/json");
	checkErrorResponse(response, 415, "application/xml");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, deleteProgram) {
	const IdType programId;
	const shared_ptr<MockProgramContainer> programContainer(new MockProgramContainer());

	EXPECT_CALL(*programContainer, erase(programId));
	EXPECT_CALL(*programContainer, insert(_, _)).Times(AnyNumber());

	document = IrrigationDocument::Builder().setProgramContainer(programContainer).build();
	document->addView(unique_ptr<View>(new RestView(*document, port)));
	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("DELETE", createProgramUrl(programId));
	checkResponseWithoutBody(response, 200);
	EXPECT_THAT(response.writeCallbackData.text, IsEmpty());
}

TEST_F(RestViewTest, deleteProgramNotFound) {
	Response response = executeRequest("DELETE", createProgramUrl(IdType()));
	checkErrorResponse(response, 404, "application/xml");
}
