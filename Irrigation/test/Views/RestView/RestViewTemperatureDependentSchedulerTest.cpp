#include "RestViewTemperatureDependentSchedulerTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewTemperatureDependentSchedulerTest::programId(25);
const RestViewTestSamples::TemperatureDependentSchedulerSample RestViewTemperatureDependentSchedulerTest::sample = RestViewTestSamples::TemperatureDependentSchedulerSampleList().front();

///////////////////////////////////////////////////////////////////////////////

void RestViewTemperatureDependentSchedulerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockSchedulerContainer = std::make_shared<MockSchedulerContainer>();
	mockTemperatureDependentScheduler = std::make_shared<StrictMock<MockTemperatureDependentScheduler>>();

	ON_CALL(*mockIrrigationDocument, getProgramContainer()).
			WillByDefault(ReturnRef(*mockProgramContainer));

	ON_CALL(*mockProgramContainer, at(programId)).
			WillByDefault(Return(mockProgram));

	ON_CALL(*mockProgram, getSchedulerContainer()).
			WillByDefault(ReturnRef(*mockSchedulerContainer));

	ON_CALL(*mockSchedulerContainer, getTemperatureDependentScheduler()).
			WillByDefault(ReturnRef(*mockTemperatureDependentScheduler));

	ON_CALL(*mockTemperatureDependentScheduler, toTemperatureDependentSchedulerDto()).
			WillByDefault(Return(sample.getDto()));
}

void RestViewTemperatureDependentSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureDependentSchedulerTest, GET) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(2);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(2);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(2);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(2);
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(2);
	EXPECT_CALL(*mockSchedulerContainer, getTemperatureDependentScheduler()).Times(2);
	EXPECT_CALL(*mockTemperatureDependentScheduler, toTemperatureDependentSchedulerDto()).Times(2);

	checkResponse_200_OK(
			GET(createTemperatureDependentSchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createTemperatureDependentSchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewTemperatureDependentSchedulerTest, GET_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createTemperatureDependentSchedulerUrl(programId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureDependentSchedulerTest, PATCH) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1);
	EXPECT_CALL(*mockSchedulerContainer, getTemperatureDependentScheduler()).Times(1);
	EXPECT_CALL(*mockTemperatureDependentScheduler, updateFromTemperatureDependentSchedulerDto(sample.getDto())).Times(1);
	EXPECT_CALL(*mockTemperatureDependentScheduler, toString()).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createTemperatureDependentSchedulerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewTemperatureDependentSchedulerTest, PATCH_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createTemperatureDependentSchedulerUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTemperatureDependentSchedulerTest, DELETE) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createTemperatureDependentSchedulerUrl(programId))
		);
}
