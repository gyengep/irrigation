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

void RestViewStartTimeContainerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
	mockProgram = std::make_shared<StrictMock<MockProgram>>();
	mockStartTimeContainer = std::make_shared<StrictMock<MockStartTimeContainer>>();

	ON_CALL(*mockIrrigationDocument, getProgramContainer()).
			WillByDefault(ReturnRef(*mockProgramContainer));

	ON_CALL(*mockProgramContainer, at(programId)).
			WillByDefault(Return(mockProgram));

	ON_CALL(*mockProgram, getStartTimeContainer()).
			WillByDefault(ReturnRef(*mockStartTimeContainer));

	ON_CALL(*mockStartTimeContainer, toStartTimeDtoList()).
			WillByDefault(Return(sample.getDtoList()));
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

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1);
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(1);
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

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			POST_ContentType_Xml(createStartTimeContainerUrl(programId), startTimeSample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeContainerTest, GET) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(2);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(2);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(2);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(2);
	EXPECT_CALL(*mockProgram, getStartTimeContainer()).Times(2);
	EXPECT_CALL(*mockStartTimeContainer, toStartTimeDtoList()).Times(2);

	checkResponse_200_OK(
			GET(createStartTimeContainerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createStartTimeContainerUrl(programId)),
			prependXmlHeader(sample.getXml())
		);
}

TEST_F(RestViewStartTimeContainerTest, GET_ProgramNotExist) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, at(programId)).Times(1).WillOnce(Throw(NoSuchElementException("")));

	checkResponse_404_Not_Found(
			GET_Accept_Xml(createStartTimeContainerUrl(programId))
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewStartTimeContainerTest, DELETE) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createStartTimeContainerUrl(programId))
		);
}
