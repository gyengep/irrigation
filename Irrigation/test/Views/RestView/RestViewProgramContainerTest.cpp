#include "RestViewProgramContainerTest.h"
#include "RestViewProgramTest.h"
#include "Request.h"
#include "Exceptions/Exceptions.h"
#include "Mocks/MockProgram.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const RestViewTestSamples::ProgramContainerSample RestViewProgramContainerTest::sample = RestViewTestSamples::ProgramContainerSampleList().back();
const RestViewTestSamples::ProgramSample RestViewProgramContainerTest::programSample = RestViewTestSamples::ProgramSampleList().back();

///////////////////////////////////////////////////////////////////////////////

void RestViewProgramContainerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();

	ON_CALL(*mockIrrigationDocument, getProgramContainer()).
			WillByDefault(ReturnRef(*mockProgramContainer));

	ON_CALL(*mockProgramContainer, toProgramDtoList()).
			WillByDefault(Return(sample.getDtoList()));
}

void RestViewProgramContainerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramContainerTest, POST) {
	IdType programId(951);
	ProgramContainer::value_type result = std::make_pair(programId, std::make_shared<NiceMock<MockProgram>>());

	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1);
	EXPECT_CALL(*mockProgramContainer, createFromProgramDto(programSample.getDto())).Times(1).WillOnce(ReturnRef(result));

	checkResponse_201_Created(
			POST_ContentType_Xml(createProgramContainerUrl(), programSample.getXml()),
			RestViewProgramTest::createProgramLocation(programId)
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramContainerTest, GET) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(2);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(2);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(2);
	EXPECT_CALL(*mockProgramContainer, toProgramDtoList()).Times(2);

	checkResponse_200_OK(
			GET(createProgramContainerUrl()),
			prependXmlAndStyleSheetHeader(sample.getXmlWithoutContainers(), "/programlist.xsl")
		);

	checkResponse_200_OK(
			GET_Accept_Xml(createProgramContainerUrl()),
			prependXmlAndStyleSheetHeader(sample.getXmlWithoutContainers(), "/programlist.xsl")
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramContainerTest, DELETE) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createProgramContainerUrl())
		);
}
