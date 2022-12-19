#include "RestViewHotWeatherSchedulerTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewHotWeatherSchedulerTest::programId(25);
const Dto2XmlTest::HotWeatherSchedulerSample RestViewHotWeatherSchedulerTest::sample = Dto2XmlTest::HotWeatherSchedulerSampleList().front();

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

TEST_F(RestViewHotWeatherSchedulerTest, post) {
	checkResponse_405_Method_Not_Allowed(
			POST_ContentType_Xml(createHotWeatherSchedulerUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewHotWeatherSchedulerTest, get) {
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

TEST_F(RestViewHotWeatherSchedulerTest, get_WithAcceptHeader) {
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

TEST_F(RestViewHotWeatherSchedulerTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createHotWeatherSchedulerUrl(programId))
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, get_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET(createHotWeatherSchedulerUrl(programId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewHotWeatherSchedulerTest, patch) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1).WillOnce(ReturnRef(*mockSchedulerContainer));
	EXPECT_CALL(*mockSchedulerContainer, getHotWeatherScheduler()).Times(1).WillOnce(ReturnRef(*mockHotWeatherScheduler));
	EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(sample.getDto())).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createHotWeatherSchedulerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, patch_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createHotWeatherSchedulerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, patch_InvalidXml) {
	checkResponse_400_Bad_Request(
			PATCH_ContentType_Xml(createHotWeatherSchedulerUrl(programId), "Invalid XML")
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, patch_InvalidContentType) {
	checkResponse_415_Unsupported_Media_Type(
			PATCH_ContentType_Json(createHotWeatherSchedulerUrl(programId), "{ \"key\" : \"value\" }")
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewHotWeatherSchedulerTest, delete) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createHotWeatherSchedulerUrl(programId))
		);
}
