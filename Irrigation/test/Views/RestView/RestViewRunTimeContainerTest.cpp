#include "RestViewRunTimeContainerTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewRunTimeContainerTest::programId(25);
const RestViewTestSamples::RunTimeContainerSample RestViewRunTimeContainerTest::sample = RestViewTestSamples::RunTimeContainerSampleList().front();

///////////////////////////////////////////////////////////////////////////////

void RestViewRunTimeContainerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockRunTimeContainer = std::make_shared<StrictMock<MockRunTimeContainer>>();

	ON_CALL(*mockIrrigationDocument, getProgramContainer()).
			WillByDefault(ReturnRef(*mockProgramContainer));

	ON_CALL(*mockProgramContainer, at(programId)).
			WillByDefault(Return(mockProgram));

	ON_CALL(*mockProgram, getRunTimeContainer()).
			WillByDefault(ReturnRef(*mockRunTimeContainer));

	ON_CALL(*mockRunTimeContainer, toRunTimeDtoList()).
			WillByDefault(Return(sample.getDtoList()));
}

void RestViewRunTimeContainerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewRunTimeContainerTest, GET) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(2);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(2);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(2);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(2);
	EXPECT_CALL(*mockProgram, getRunTimeContainer()).Times(2);
	EXPECT_CALL(*mockRunTimeContainer, toRunTimeDtoList()).Times(2);

	checkResponse_200_OK(
			GET(createRunTimeContainerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createRunTimeContainerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewRunTimeContainerTest, GET_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createRunTimeContainerUrl(programId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewRunTimeContainerTest, PATCH) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, getRunTimeContainer()).Times(1);
	EXPECT_CALL(*mockRunTimeContainer, updateFromRunTimeDtoList(sample.getDtoList())).Times(1);
	EXPECT_CALL(*mockRunTimeContainer, toString()).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createRunTimeContainerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewRunTimeContainerTest, PATCH_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createRunTimeContainerUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewRunTimeContainerTest, DELETE) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createRunTimeContainerUrl(programId))
		);
}
