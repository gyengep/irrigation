#include "RestViewWeeklySchedulerTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewWeeklySchedulerTest::programId(25);
const RestViewTestSamples::WeeklySchedulerSample RestViewWeeklySchedulerTest::sample = RestViewTestSamples::WeeklySchedulerSampleList().front();

///////////////////////////////////////////////////////////////////////////////

std::string RestViewWeeklySchedulerTest::createWeeklySchedulerUrl(IdType programId) {
	return createUrl("/programs/" + programId.toString() + "/schedulers/weekly");
}

void RestViewWeeklySchedulerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockSchedulerContainer = std::make_shared<MockSchedulerContainer>();
	mockWeeklyScheduler = std::make_shared<StrictMock<MockWeeklyScheduler>>();
}

void RestViewWeeklySchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewWeeklySchedulerTest, post) {
	checkResponse_405_Method_Not_Allowed(
			POST_ContentType_Xml(createWeeklySchedulerUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewWeeklySchedulerTest, get) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1).WillOnce(ReturnRef(*mockSchedulerContainer));
	EXPECT_CALL(*mockSchedulerContainer, getWeeklyScheduler()).Times(1).WillOnce(ReturnRef(*mockWeeklyScheduler));
	EXPECT_CALL(*mockWeeklyScheduler, toWeeklySchedulerDto()).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET(createWeeklySchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewWeeklySchedulerTest, get_WithAcceptHeader) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1).WillOnce(ReturnRef(*mockSchedulerContainer));
	EXPECT_CALL(*mockSchedulerContainer, getWeeklyScheduler()).Times(1).WillOnce(ReturnRef(*mockWeeklyScheduler));
	EXPECT_CALL(*mockWeeklyScheduler, toWeeklySchedulerDto()).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET_Accept_Xml(createWeeklySchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewWeeklySchedulerTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createWeeklySchedulerUrl(programId))
		);
}

TEST_F(RestViewWeeklySchedulerTest, get_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createWeeklySchedulerUrl(programId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewWeeklySchedulerTest, patch) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1).WillOnce(ReturnRef(*mockSchedulerContainer));
	EXPECT_CALL(*mockSchedulerContainer, getWeeklyScheduler()).Times(1).WillOnce(ReturnRef(*mockWeeklyScheduler));
	EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(sample.getDto())).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createWeeklySchedulerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewWeeklySchedulerTest, patch_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createWeeklySchedulerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewWeeklySchedulerTest, patch_InvalidXml) {
	checkResponse_400_Bad_Request(
			PATCH_ContentType_Xml(createWeeklySchedulerUrl(programId), "Invalid XML")
		);
}

TEST_F(RestViewWeeklySchedulerTest, patch_InvalidContentType) {
	checkResponse_415_Unsupported_Media_Type(
			PATCH_ContentType_Json(createWeeklySchedulerUrl(programId), "{ \"key\" : \"value\" }")
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewWeeklySchedulerTest, delete) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createWeeklySchedulerUrl(programId))
		);
}
