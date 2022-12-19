#include "RestViewTemperatureDependentSchedulerTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewTemperatureDependentSchedulerTest::programId(25);
const Dto2XmlTest::TemperatureDependentSchedulerSample RestViewTemperatureDependentSchedulerTest::sample = Dto2XmlTest::TemperatureDependentSchedulerSampleList().front();

///////////////////////////////////////////////////////////////////////////////

std::string RestViewTemperatureDependentSchedulerTest::createTemperatureDependentSchedulerUrl(IdType programId) {
	return createUrl("/programs/" + programId.toString() + "/schedulers/temperature-dependent");
}

void RestViewTemperatureDependentSchedulerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockSchedulerContainer = std::make_shared<MockSchedulerContainer>();
	mockTemperatureDependentScheduler = std::make_shared<StrictMock<MockTemperatureDependentScheduler>>();
}

void RestViewTemperatureDependentSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureDependentSchedulerTest, post) {
	checkResponse_405_Method_Not_Allowed(
			POST_ContentType_Xml(createTemperatureDependentSchedulerUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureDependentSchedulerTest, get) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1).WillOnce(ReturnRef(*mockSchedulerContainer));
	EXPECT_CALL(*mockSchedulerContainer, getTemperatureDependentScheduler()).Times(1).WillOnce(ReturnRef(*mockTemperatureDependentScheduler));
	EXPECT_CALL(*mockTemperatureDependentScheduler, toTemperatureDependentSchedulerDto()).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET(createTemperatureDependentSchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewTemperatureDependentSchedulerTest, get_WithAcceptHeader) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1).WillOnce(ReturnRef(*mockSchedulerContainer));
	EXPECT_CALL(*mockSchedulerContainer, getTemperatureDependentScheduler()).Times(1).WillOnce(ReturnRef(*mockTemperatureDependentScheduler));
	EXPECT_CALL(*mockTemperatureDependentScheduler, toTemperatureDependentSchedulerDto()).Times(1).WillOnce(Return(sample.getDto()));

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureDependentSchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewTemperatureDependentSchedulerTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createTemperatureDependentSchedulerUrl(programId))
		);
}

TEST_F(RestViewTemperatureDependentSchedulerTest, get_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createTemperatureDependentSchedulerUrl(programId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureDependentSchedulerTest, patch) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Return(mockProgram));
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1).WillOnce(ReturnRef(*mockSchedulerContainer));
	EXPECT_CALL(*mockSchedulerContainer, getTemperatureDependentScheduler()).Times(1).WillOnce(ReturnRef(*mockTemperatureDependentScheduler));
	EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(sample.getDto())).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createTemperatureDependentSchedulerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewTemperatureDependentSchedulerTest, patch_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createTemperatureDependentSchedulerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewTemperatureDependentSchedulerTest, patch_InvalidXml) {
	checkResponse_400_Bad_Request(
			PATCH_ContentType_Xml(createTemperatureDependentSchedulerUrl(programId), "Invalid XML")
		);
}

TEST_F(RestViewTemperatureDependentSchedulerTest, patch_InvalidContentType) {
	checkResponse_415_Unsupported_Media_Type(
			PATCH_ContentType_Json(createTemperatureDependentSchedulerUrl(programId), "{ \"key\" : \"value\" }")
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureDependentSchedulerTest, delete) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createTemperatureDependentSchedulerUrl(programId))
		);
}
