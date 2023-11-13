#include "RestViewStartTimeContainerTest.h"
#include "RestViewStartTimeTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"
#include "Mocks/MockStartTime.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewStartTimeContainerTest::programId(25);
const RestViewTestSamples::StartTimeContainerSample RestViewStartTimeContainerTest::sample = RestViewTestSamples::StartTimeContainerSampleList().back();
const RestViewTestSamples::StartTimeSample RestViewStartTimeContainerTest::startTimeSample = RestViewTestSamples::StartTimeSampleList().back();

///////////////////////////////////////////////////////////////////////////////

std::string RestViewStartTimeContainerTest::createStartTimeContainerUrl(IdType programId) {
	return createUrl("/programs/" + programId.toString() + "/starttimes");
}

void RestViewStartTimeContainerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockStartTimeContainer = std::make_shared<StrictMock<MockStartTimeContainer>>();
}

void RestViewStartTimeContainerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeContainerTest, POST) {
	IdType startTimeId(951);
	StartTimeContainer::value_type result = std::make_pair(startTimeId, std::make_shared<NiceMock<MockStartTime>>());

	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1).WillOnce(ReturnRef(*mockStartTimeContainer));
	EXPECT_CALL(*mockStartTimeContainer, createFromStartTimeDto(startTimeSample.getDto())).Times(1).WillOnce(ReturnRef(result));

	checkResponse_201_Created(
			POST_ContentType_Xml(createStartTimeContainerUrl(programId), startTimeSample.getXml()),
			RestViewStartTimeTest::createStartTimeLocation(programId, startTimeId)
		);
}

TEST_F(RestViewStartTimeContainerTest, POST_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			POST_ContentType_Xml(createStartTimeContainerUrl(programId), startTimeSample.getXml())
		);
}

TEST_F(RestViewStartTimeContainerTest, POST_InvalidXml) {
	checkResponse_400_Bad_Request(
			POST_ContentType_Xml(createStartTimeContainerUrl(programId), "Invalid XML")
		);
}

TEST_F(RestViewStartTimeContainerTest, POST_InvalidContentType) {
	checkResponse_415_Unsupported_Media_Type(
			POST_ContentType_Json(createStartTimeContainerUrl(programId), "{ \"key\" : \"value\" }")
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeContainerTest, GET) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1).WillOnce(ReturnRef(*mockStartTimeContainer));
	EXPECT_CALL(*mockStartTimeContainer, toStartTimeDtoList()).Times(1).WillOnce(Return(sample.getDtoList()));

	checkResponse_200_OK(
			GET(createStartTimeContainerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewStartTimeContainerTest, GET_WithAcceptHeader) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1).WillOnce(ReturnRef(*mockStartTimeContainer));
	EXPECT_CALL(*mockStartTimeContainer, toStartTimeDtoList()).Times(1).WillOnce(Return(sample.getDtoList()));

	checkResponse_200_OK(
			GET_Accept_Xml(createStartTimeContainerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewStartTimeContainerTest, GET_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createStartTimeContainerUrl(programId))
		);
}

TEST_F(RestViewStartTimeContainerTest, GET_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createStartTimeContainerUrl(programId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeContainerTest, PATCH) {
	checkResponse_405_Method_Not_Allowed(
			PATCH_ContentType_Xml(createStartTimeContainerUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeContainerTest, DELETE) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createStartTimeContainerUrl(programId))
		);
}
