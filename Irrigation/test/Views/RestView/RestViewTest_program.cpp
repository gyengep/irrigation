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
	EXPECT_THAT(response.responseCode, Eq(200));
	EXPECT_THAT(response.writeCallbackData.text, Eq(XmlWriter().save(programDto)));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, postProgram) {
	Response response = executeRequest("POST", createProgramUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(405));
}

TEST_F(RestViewTest, getProgram) {
	document->getPrograms().insert(1, ProgramSample1().getObject());
	document->getPrograms().insert(2, ProgramSample2().getObject());
	document->getPrograms().insert(3, ProgramSample3().getObject());
	document->getPrograms().insert(4, ProgramSample4().getObject());

	testGetProgram(1, ProgramSample1().getDto());
	testGetProgram(2, ProgramSample2().getDto());
	testGetProgram(3, ProgramSample3().getDto());
	testGetProgram(4, ProgramSample4().getDto());
}

TEST_F(RestViewTest, getProgramNotFound) {
	Response response = executeRequest("GET", createProgramUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(404));
}

TEST_F(RestViewTest, putProgram) {
	Response response = executeRequest("PUT", createProgramUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(405));
}

TEST_F(RestViewTest, patchProgram) {
	const IdType programId;
	const ProgramSample4 programSample;
	const shared_ptr<MockProgram> program(new MockProgram());

	EXPECT_CALL(*program, updateFromProgramDto(programSample.getDto()));

	document->getPrograms().insert(programId, program);

	Response response = executeRequest("PATCH", createProgramUrl(programId), XmlWriter().save(programSample.getDto()));
	EXPECT_THAT(response.responseCode, Eq(204));
}

TEST_F(RestViewTest, patchProgramNotFound) {
	Response response = executeRequest("PATCH", createProgramUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(404));
}

TEST_F(RestViewTest, patchProgramInvalidXml) {
	const IdType programId;

	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	const Response response = executeRequest("PATCH", createProgramUrl(programId), "InvalidXml");
	EXPECT_THAT(response.responseCode, Eq(400));
}

TEST_F(RestViewTest, deleteProgram) {
	const IdType programId;
	const shared_ptr<MockProgramContainer> programContainer(new MockProgramContainer());

	EXPECT_CALL(*programContainer, erase(programId));
	EXPECT_CALL(*programContainer, insert(_, _)).Times(AnyNumber());

	document = IrrigationDocument::Builder().setProgramContainer(programContainer).build();
	document->addView(unique_ptr<View>(new RestView(*document, port)));
	document->getPrograms().insert(programId, shared_ptr<Program>(new Program()));

	Response response = executeRequest("DELETE", createProgramUrl(programId));
	EXPECT_THAT(response.responseCode, Eq(200));
}

TEST_F(RestViewTest, deleteProgramNotFound) {
	Response response = executeRequest("DELETE", createProgramUrl(IdType()));
	EXPECT_THAT(response.responseCode, Eq(404));
}
