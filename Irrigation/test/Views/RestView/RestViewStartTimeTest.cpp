#include "RestViewStartTimeTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewStartTimeTest::programId(25);
const IdType RestViewStartTimeTest::startTimeId(10);
const RestViewTestSamples::StartTimeSample RestViewStartTimeTest::sample = RestViewTestSamples::StartTimeSampleList().front();

///////////////////////////////////////////////////////////////////////////////

std::string RestViewStartTimeTest::createStartTimeLocation(IdType programId, IdType startTimeId) {
	return "/programs/" + programId.toString() + "/starttimes/" + startTimeId.toString();
}

std::string RestViewStartTimeTest::createStartTimeUrl(IdType programId, IdType startTimeId) {
	return createUrl(createStartTimeLocation(programId, startTimeId));
}

void RestViewStartTimeTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockStartTimeContainer = std::make_shared<StrictMock<MockStartTimeContainer>>();
	mockStartTime = std::make_shared<StrictMock<MockStartTime>>();
}

void RestViewStartTimeTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeTest, post) {
	checkResponse_405_Method_Not_Allowed(
			POST_ContentType_Xml(createStartTimeUrl(programId, startTimeId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeTest, get) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1).WillOnce(ReturnRef(*mockStartTimeContainer));
	EXPECT_CALL(*mockStartTimeContainer, at(startTimeId)).Times(1).WillOnce(Return(mockStartTime));
	EXPECT_CALL(*mockStartTime, toStartTimeDto()).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET(createStartTimeUrl(programId, startTimeId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewStartTimeTest, get_WithAcceptHeader) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1).WillOnce(ReturnRef(*mockStartTimeContainer));
	EXPECT_CALL(*mockStartTimeContainer, at(startTimeId)).Times(1).WillOnce(Return(mockStartTime));
	EXPECT_CALL(*mockStartTime, toStartTimeDto()).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET_Accept_Xml(createStartTimeUrl(programId, startTimeId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewStartTimeTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createStartTimeUrl(programId, startTimeId))
		);
}

TEST_F(RestViewStartTimeTest, get_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createStartTimeUrl(programId, startTimeId))
		);
}

TEST_F(RestViewStartTimeTest, get_StartTimeNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1).WillOnce(ReturnRef(*mockStartTimeContainer));
	EXPECT_CALL(*mockStartTimeContainer, at(startTimeId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createStartTimeUrl(programId, startTimeId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeTest, patch) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1).WillOnce(ReturnRef(*mockStartTimeContainer));
	EXPECT_CALL(*mockStartTimeContainer, at(startTimeId)).Times(1).WillOnce(Return(mockStartTime));
	EXPECT_CALL(*mockStartTime, updateFromStartTimeDto(sample.getDto())).Times(1);
	EXPECT_CALL(*mockStartTime, toString()).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createStartTimeUrl(programId, startTimeId), sample.getXml())
		);
}

TEST_F(RestViewStartTimeTest, patch_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createStartTimeUrl(programId, startTimeId), sample.getXml())
		);
}

TEST_F(RestViewStartTimeTest, patch_StartTimeNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1).WillOnce(ReturnRef(*mockStartTimeContainer));
	EXPECT_CALL(*mockStartTimeContainer, at(startTimeId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createStartTimeUrl(programId, startTimeId), sample.getXml())
		);
}

TEST_F(RestViewStartTimeTest, patch_InvalidXml) {
	checkResponse_400_Bad_Request(
			PATCH_ContentType_Xml(createStartTimeUrl(programId, startTimeId), "Invalid XML")
		);
}

TEST_F(RestViewStartTimeTest, patch_InvalidContentType) {
	checkResponse_415_Unsupported_Media_Type(
			PATCH_ContentType_Json(createStartTimeUrl(programId, startTimeId), "{ \"key\" : \"value\" }")
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeTest, delete) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1).WillOnce(ReturnRef(*mockStartTimeContainer));
	EXPECT_CALL(*mockStartTimeContainer, erase(startTimeId)).Times(1);

	checkResponse_204_No_Content(
			DELETE(createStartTimeUrl(programId, startTimeId))
		);
}

TEST_F(RestViewStartTimeTest, delete_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			DELETE(createStartTimeUrl(programId, startTimeId))
		);
}

TEST_F(RestViewStartTimeTest, delete_StartTimeNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1).WillOnce(ReturnRef(*mockStartTimeContainer));
	EXPECT_CALL(*mockStartTimeContainer, erase(startTimeId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			DELETE(createStartTimeUrl(programId, startTimeId))
		);
}
