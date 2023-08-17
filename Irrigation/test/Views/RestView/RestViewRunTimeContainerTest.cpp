#include "RestViewRunTimeContainerTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewRunTimeContainerTest::programId(25);
const RestViewTestSamples::RunTimeContainerSample RestViewRunTimeContainerTest::sample = RestViewTestSamples::RunTimeContainerSampleList().front();

///////////////////////////////////////////////////////////////////////////////

std::string RestViewRunTimeContainerTest::createRunTimeContainerUrl(IdType programId) {
	return createUrl("/programs/" + programId.toString() + "/runtimes");
}

void RestViewRunTimeContainerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockRunTimeContainer = std::make_shared<StrictMock<MockRunTimeContainer>>();
}

void RestViewRunTimeContainerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewRunTimeContainerTest, post) {
	checkResponse_405_Method_Not_Allowed(
			POST_ContentType_Xml(createRunTimeContainerUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewRunTimeContainerTest, get) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getRunTimeContainer()).Times(1).WillOnce(ReturnRef(*mockRunTimeContainer));
	EXPECT_CALL(*mockRunTimeContainer, toRunTimeDtoList()).Times(1).WillOnce(Return(sample.getDtoList()));

	checkResponse_200_OK(
			GET(createRunTimeContainerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewRunTimeContainerTest, get_WithAcceptHeader) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getRunTimeContainer()).Times(1).WillOnce(ReturnRef(*mockRunTimeContainer));
	EXPECT_CALL(*mockRunTimeContainer, toRunTimeDtoList()).Times(1).WillOnce(Return(sample.getDtoList()));

	checkResponse_200_OK(
			GET_Accept_Xml(createRunTimeContainerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewRunTimeContainerTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createRunTimeContainerUrl(programId))
		);
}

TEST_F(RestViewRunTimeContainerTest, get_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createRunTimeContainerUrl(programId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewRunTimeContainerTest, patch) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getRunTimeContainer()).Times(1).WillOnce(ReturnRef(*mockRunTimeContainer));
	EXPECT_CALL(*mockRunTimeContainer, updateFromRunTimeDtoList(sample.getDtoList())).Times(1);
	EXPECT_CALL(*mockRunTimeContainer, toString()).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createRunTimeContainerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewRunTimeContainerTest, patch_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createRunTimeContainerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewRunTimeContainerTest, patch_InvalidXml) {
	checkResponse_400_Bad_Request(
			PATCH_ContentType_Xml(createRunTimeContainerUrl(programId), "Invalid XML")
		);
}

TEST_F(RestViewRunTimeContainerTest, patch_InvalidContentType) {
	checkResponse_415_Unsupported_Media_Type(
			PATCH_ContentType_Json(createRunTimeContainerUrl(programId), "{ \"key\" : \"value\" }")
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewRunTimeContainerTest, delete) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createRunTimeContainerUrl(programId))
		);
}
