#include "RestViewProgramTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewProgramTest::programId(25);
const RestViewTestSamples::ProgramSample RestViewProgramTest::sample = RestViewTestSamples::ProgramSampleList().back();

///////////////////////////////////////////////////////////////////////////////

std::string RestViewProgramTest::createProgramLocation(IdType programId) {
	return "/programs/" + programId.toString();
}

std::string RestViewProgramTest::createProgramUrl(IdType programId) {
	return createUrl(createProgramLocation(programId));
}

void RestViewProgramTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
}

void RestViewProgramTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramTest, POST) {
	checkResponse_405_Method_Not_Allowed(
			POST_ContentType_Xml(createProgramUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramTest, GET) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, toProgramDto()).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET(createProgramUrl(programId)),
			prependXmlAndStyleSheetHeader(sample.getXml(), "/program.xsl")
		);
}

TEST_F(RestViewProgramTest, GET_WithAcceptHeader) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, toProgramDto()).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET_Accept_Xml(createProgramUrl(programId)),
			prependXmlAndStyleSheetHeader(sample.getXml(), "/program.xsl")
		);
}

TEST_F(RestViewProgramTest, GET_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createProgramUrl(programId))
		);
}

TEST_F(RestViewProgramTest, GET_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
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

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, updateFromProgramDto(sample.getDto())).Times(1);
	EXPECT_CALL(*mockProgram, toString()).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createProgramUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewProgramTest, PATCH_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createProgramUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewProgramTest, PATCH_InvalidXml) {
	checkResponse_400_Bad_Request(
			PATCH_ContentType_Xml(createProgramUrl(programId), "Invalid XML")
		);
}

TEST_F(RestViewProgramTest, PATCH_InvalidContentType) {
	checkResponse_415_Unsupported_Media_Type(
			PATCH_ContentType_Json(createProgramUrl(programId), "{ \"key\" : \"value\" }")
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramTest, DELETE) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, erase(programId)).Times(1);

	checkResponse_204_No_Content(
			DELETE(createProgramUrl(programId))
		);
}

TEST_F(RestViewProgramTest, DELETE_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, erase(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			DELETE(createProgramUrl(programId))
		);
}
