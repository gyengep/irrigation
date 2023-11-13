#include "RestViewHotWeatherSchedulerTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewHotWeatherSchedulerTest::programId(25);
const RestViewTestSamples::HotWeatherSchedulerSample RestViewHotWeatherSchedulerTest::sample = RestViewTestSamples::HotWeatherSchedulerSampleList().front();

///////////////////////////////////////////////////////////////////////////////

std::string RestViewHotWeatherSchedulerTest::createHotWeatherSchedulerUrl(IdType programId) {
	return createUrl("/programs/" + programId.toString() + "/schedulers/hot-weather");
}

void RestViewHotWeatherSchedulerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockSchedulerContainer = std::make_shared<MockSchedulerContainer>();
	mockHotWeatherScheduler = std::make_shared<StrictMock<MockHotWeatherScheduler>>();
}

void RestViewHotWeatherSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewHotWeatherSchedulerTest, POST) {
	checkResponse_405_Method_Not_Allowed(
			POST_ContentType_Xml(createHotWeatherSchedulerUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewHotWeatherSchedulerTest, GET) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1).WillOnce(ReturnRef(*mockSchedulerContainer));
	EXPECT_CALL(*mockSchedulerContainer, getHotWeatherScheduler()).Times(1).WillOnce(ReturnRef(*mockHotWeatherScheduler));
	EXPECT_CALL(*mockHotWeatherScheduler, toHotWeatherSchedulerDto()).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET(createHotWeatherSchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, GET_WithAcceptHeader) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1).WillOnce(ReturnRef(*mockSchedulerContainer));
	EXPECT_CALL(*mockSchedulerContainer, getHotWeatherScheduler()).Times(1).WillOnce(ReturnRef(*mockHotWeatherScheduler));
	EXPECT_CALL(*mockHotWeatherScheduler, toHotWeatherSchedulerDto()).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET_Accept_Xml(createHotWeatherSchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, GET_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createHotWeatherSchedulerUrl(programId))
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, GET_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET(createHotWeatherSchedulerUrl(programId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewHotWeatherSchedulerTest, PATCH) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1).WillOnce(ReturnRef(*mockSchedulerContainer));
	EXPECT_CALL(*mockSchedulerContainer, getHotWeatherScheduler()).Times(1).WillOnce(ReturnRef(*mockHotWeatherScheduler));
	EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(sample.getDto())).Times(1);
	EXPECT_CALL(*mockHotWeatherScheduler, toString()).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createHotWeatherSchedulerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, PATCH_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createHotWeatherSchedulerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, PATCH_InvalidXml) {
	checkResponse_400_Bad_Request(
			PATCH_ContentType_Xml(createHotWeatherSchedulerUrl(programId), "Invalid XML")
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, PATCH_InvalidContentType) {
	checkResponse_415_Unsupported_Media_Type(
			PATCH_ContentType_Json(createHotWeatherSchedulerUrl(programId), "{ \"key\" : \"value\" }")
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewHotWeatherSchedulerTest, DELETE) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createHotWeatherSchedulerUrl(programId))
		);
}
