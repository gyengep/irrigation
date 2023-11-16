#include "RestViewProgramTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewProgramTest::programId(25);
const RestViewTestSamples::ProgramSample RestViewProgramTest::sample = RestViewTestSamples::ProgramSampleList().back();

///////////////////////////////////////////////////////////////////////////////

void RestViewProgramTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();

	ON_CALL(*mockIrrigationDocument, getProgramContainer()).
			WillByDefault(ReturnRef(*mockProgramContainer));

	ON_CALL(*mockProgramContainer, at(programId)).
			WillByDefault(Return(mockProgram));

	ON_CALL(*mockProgram, toProgramDto()).
			WillByDefault(Return(sample.getDto()));
}

void RestViewProgramTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramTest, GET) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(2);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(2);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(2);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(2);
	EXPECT_CALL(*mockProgram, toProgramDto()).Times(2);

	checkResponse_200_OK(
			GET(createProgramUrl(programId)),
			prependXmlAndStyleSheetHeader(sample.getXml(), "/program.xsl")
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createProgramUrl(programId)),
			prependXmlAndStyleSheetHeader(sample.getXml(), "/program.xsl")
		);
}

TEST_F(RestViewProgramTest, GET_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createProgramUrl(programId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramTest, PATCH) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, updateFromProgramDto(sample.getDto())).Times(1);
	EXPECT_CALL(*mockProgram, toString()).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createProgramUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewProgramTest, PATCH_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createProgramUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramTest, DELETE) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, erase(programId)).Times(1);

	checkResponse_204_No_Content(
			DELETE(createProgramUrl(programId))
		);
}

TEST_F(RestViewProgramTest, DELETE_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, erase(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			DELETE(createProgramUrl(programId))
		);
}
