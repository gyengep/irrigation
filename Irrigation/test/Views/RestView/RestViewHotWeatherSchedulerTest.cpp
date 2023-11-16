#include "RestViewHotWeatherSchedulerTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewHotWeatherSchedulerTest::programId(25);
const RestViewTestSamples::HotWeatherSchedulerSample RestViewHotWeatherSchedulerTest::sample = RestViewTestSamples::HotWeatherSchedulerSampleList().front();

///////////////////////////////////////////////////////////////////////////////

void RestViewHotWeatherSchedulerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockSchedulerContainer = std::make_shared<MockSchedulerContainer>();
	mockHotWeatherScheduler = std::make_shared<StrictMock<MockHotWeatherScheduler>>();

	ON_CALL(*mockIrrigationDocument, getProgramContainer()).
			WillByDefault(ReturnRef(*mockProgramContainer));

	ON_CALL(*mockProgramContainer, at(programId)).
			WillByDefault(Return(mockProgram));

	ON_CALL(*mockProgram, getSchedulerContainer()).
			WillByDefault(ReturnRef(*mockSchedulerContainer));

	ON_CALL(*mockSchedulerContainer, getHotWeatherScheduler()).
			WillByDefault(ReturnRef(*mockHotWeatherScheduler));

	ON_CALL(*mockHotWeatherScheduler, toHotWeatherSchedulerDto()).
			WillByDefault(Return(sample.getDto()));
}

void RestViewHotWeatherSchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewHotWeatherSchedulerTest, GET) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(2);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(2);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(2);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(2);
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(2);
	EXPECT_CALL(*mockSchedulerContainer, getHotWeatherScheduler()).Times(2);
	EXPECT_CALL(*mockHotWeatherScheduler, toHotWeatherSchedulerDto()).Times(2);

	checkResponse_200_OK(
			GET(createHotWeatherSchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createHotWeatherSchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, GET_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
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

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1);
	EXPECT_CALL(*mockSchedulerContainer, getHotWeatherScheduler()).Times(1);
	EXPECT_CALL(*mockHotWeatherScheduler, updateFromHotWeatherSchedulerDto(sample.getDto())).Times(1);
	EXPECT_CALL(*mockHotWeatherScheduler, toString()).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createHotWeatherSchedulerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewHotWeatherSchedulerTest, PATCH_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createHotWeatherSchedulerUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewHotWeatherSchedulerTest, DELETE) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createHotWeatherSchedulerUrl(programId))
		);
}
