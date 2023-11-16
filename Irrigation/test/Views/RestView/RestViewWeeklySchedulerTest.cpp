#include "RestViewWeeklySchedulerTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const IdType RestViewWeeklySchedulerTest::programId(25);
const RestViewTestSamples::WeeklySchedulerSample RestViewWeeklySchedulerTest::sample = RestViewTestSamples::WeeklySchedulerSampleList().front();

///////////////////////////////////////////////////////////////////////////////

void RestViewWeeklySchedulerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockSchedulerContainer = std::make_shared<MockSchedulerContainer>();
	mockWeeklyScheduler = std::make_shared<StrictMock<MockWeeklyScheduler>>();

	ON_CALL(*mockIrrigationDocument, getProgramContainer()).
			WillByDefault(ReturnRef(*mockProgramContainer));

	ON_CALL(*mockProgramContainer, at(programId)).
			WillByDefault(Return(mockProgram));

	ON_CALL(*mockProgram, getSchedulerContainer()).
			WillByDefault(ReturnRef(*mockSchedulerContainer));

	ON_CALL(*mockSchedulerContainer, getWeeklyScheduler()).
			WillByDefault(ReturnRef(*mockWeeklyScheduler));

	ON_CALL(*mockWeeklyScheduler, toWeeklySchedulerDto()).
			WillByDefault(Return(sample.getDto()));
}

void RestViewWeeklySchedulerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewWeeklySchedulerTest, GET) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(2);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(2);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(2);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(2);
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(2);
	EXPECT_CALL(*mockSchedulerContainer, getWeeklyScheduler()).Times(2);
	EXPECT_CALL(*mockWeeklyScheduler, toWeeklySchedulerDto()).Times(2);

	checkResponse_200_OK(
			GET(createWeeklySchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createWeeklySchedulerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewWeeklySchedulerTest, GET_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createWeeklySchedulerUrl(programId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewWeeklySchedulerTest, PATCH) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, getSchedulerContainer()).Times(1);
	EXPECT_CALL(*mockSchedulerContainer, getWeeklyScheduler()).Times(1);
	EXPECT_CALL(*mockWeeklyScheduler, updateFromWeeklySchedulerDto(sample.getDto())).Times(1);
	EXPECT_CALL(*mockWeeklyScheduler, toString()).Times(1);

	checkResponse_204_No_Content(
			PATCH_ContentType_Xml(createWeeklySchedulerUrl(programId), sample.getXml())
		);
}

TEST_F(RestViewWeeklySchedulerTest, PATCH_ProgramNotExits) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			PATCH_ContentType_Xml(createWeeklySchedulerUrl(programId), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewWeeklySchedulerTest, DELETE) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createWeeklySchedulerUrl(programId))
		);
}
