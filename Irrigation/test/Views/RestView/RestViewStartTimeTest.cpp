#include "RestViewStartTimeTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewStartTimeTest::programId(25);
const IdType RestViewStartTimeTest::startTimeId(10);
const RestViewStartTimeTest::StartTimeSample RestViewStartTimeTest::sample(
		StartTimeDto(10, 20),
		"<starttime><hour>10</hour><minute>20</minute></starttime>"
	);

///////////////////////////////////////////////////////////////////////////////

void RestViewStartTimeTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockStartTimeContainer = std::make_shared<StrictMock<MockStartTimeContainer>>();
	mockStartTime = std::make_shared<StrictMock<MockStartTime>>();

	ON_CALL(*mockIrrigationDocument, getProgramContainer()).
			WillByDefault(ReturnRef(*mockProgramContainer));

	ON_CALL(*mockProgramContainer, at(programId)).
			WillByDefault(Return(mockProgram));

	ON_CALL(*mockProgram, getStartTimeContainer()).
			WillByDefault(ReturnRef(*mockStartTimeContainer));

	ON_CALL(*mockStartTimeContainer, at(startTimeId)).
			WillByDefault(Return(mockStartTime));

	ON_CALL(*mockStartTime, toStartTimeDto()).
			WillByDefault(Return(sample.getDto()));
}

void RestViewStartTimeTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeTest, GET) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(2);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(2);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(2);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(2);
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(2);
	EXPECT_CALL(*mockStartTimeContainer, at(startTimeId)).Times(2);
	EXPECT_CALL(*mockStartTime, toStartTimeDto()).Times(2);

	checkResponse_200_OK(
			GET(createStartTimeUrl(programId, startTimeId)),
			prependXmlHeader(sample.getXml())
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createStartTimeUrl(programId, startTimeId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewStartTimeTest, GET_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createStartTimeUrl(programId, startTimeId))
		);
}

TEST_F(RestViewStartTimeTest, GET_StartTimeNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1);
	EXPECT_CALL(*mockStartTimeContainer, at(startTimeId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createStartTimeUrl(programId, startTimeId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeTest, PATCH) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1);
	EXPECT_CALL(*mockStartTimeContainer, at(startTimeId)).Times(1);
	EXPECT_CALL(*mockStartTime, updateFromStartTimeDto(sample.getDto())).Times(1);
	EXPECT_CALL(*mockStartTime, toString()).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createStartTimeUrl(programId, startTimeId), sample.getXml())
		);
}

TEST_F(RestViewStartTimeTest, PATCH_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createStartTimeUrl(programId, startTimeId), sample.getXml())
		);
}

TEST_F(RestViewStartTimeTest, PATCH_StartTimeNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1);
	EXPECT_CALL(*mockStartTimeContainer, at(startTimeId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createStartTimeUrl(programId, startTimeId), sample.getXml())
		);
}

TEST_F(RestViewStartTimeTest, PATCH_InvalidContent) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1);
	EXPECT_CALL(*mockStartTimeContainer, at(startTimeId)).Times(1);

	EXPECT_CALL(*mockStartTime, updateFromStartTimeDto(sample.getDto())).Times(1).WillOnce(Throw(ValueOutOfBoundsException("")));

	checkResponse_422_Unprocessable_Content(
			PATCH_ContentType_Xml(createStartTimeUrl(programId, startTimeId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeTest, DELETE) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1);
	EXPECT_CALL(*mockStartTimeContainer, erase(startTimeId)).Times(1);

	checkResponse_204_No_Content(
			DELETE(createStartTimeUrl(programId, startTimeId))
		);
}

TEST_F(RestViewStartTimeTest, DELETE_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			DELETE(createStartTimeUrl(programId, startTimeId))
		);
}

TEST_F(RestViewStartTimeTest, DELETE_StartTimeNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1);
	EXPECT_CALL(*mockStartTimeContainer, erase(startTimeId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			DELETE(createStartTimeUrl(programId, startTimeId))
		);
}
