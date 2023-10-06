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

std::string RestViewProgramContainerTest::createProgramContainerUrl() {
	return createUrl("/programs");
}

void RestViewProgramContainerTest::SetUp() {

	RestViewTest::SetUp();

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();
}

void RestViewProgramContainerTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramContainerTest, post) {
	IdType programId(951);
	ProgramContainer::value_type result = std::make_pair(programId, std::make_shared<NiceMock<MockProgram>>());

	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, setModified(true)).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, createAndInit(programSample.getDto())).Times(1).WillOnce(ReturnRef(result));

	checkResponse_201_Created(
			POST_ContentType_Xml(createProgramContainerUrl(), programSample.getXml()),
			RestViewProgramTest::createProgramLocation(programId)
		);
}

TEST_F(RestViewProgramContainerTest, patch_InvalidXml) {
	checkResponse_400_Bad_Request(
			POST_ContentType_Xml(createProgramContainerUrl(), "Invalid XML")
		);
}

TEST_F(RestViewProgramContainerTest, patch_InvalidContentType) {
	checkResponse_415_Unsupported_Media_Type(
			POST_ContentType_Json(createProgramContainerUrl(), "{ \"key\" : \"value\" }")
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramContainerTest, get) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, toProgramDtoList()).Times(1).WillOnce(Return(sample.getDtoList()));

	checkResponse_200_OK(
			GET(createProgramContainerUrl()),
			prependXmlAndStyleSheetHeader(sample.getXmlWithoutContainers(), "/programlist.xsl")
		);
}

TEST_F(RestViewProgramContainerTest, get_WithAcceptHeader) {
	EXPECT_CALL(*mockIrrigationDocument, lock()).Times(1);
	EXPECT_CALL(*mockIrrigationDocument, unlock()).Times(1);

	EXPECT_CALL(*mockIrrigationDocument, getProgramContainer()).Times(1).WillOnce(ReturnRef(*mockProgramContainer));
	EXPECT_CALL(*mockProgramContainer, toProgramDtoList()).Times(1).WillOnce(Return(sample.getDtoList()));

	checkResponse_200_OK(
			GET_Accept_Xml(createProgramContainerUrl()),
			prependXmlAndStyleSheetHeader(sample.getXmlWithoutContainers(), "/programlist.xsl")
		);
}

TEST_F(RestViewProgramContainerTest, get_NotAcceptable) {
	checkResponse_406_Not_Acceptable(
			GET_Accept_Json(createProgramContainerUrl())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramContainerTest, patch) {
	checkResponse_405_Method_Not_Allowed(
			PATCH_ContentType_Xml(createProgramContainerUrl(), sample.getXml())
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewProgramContainerTest, delete) {
	checkResponse_405_Method_Not_Allowed(
			DELETE(createProgramContainerUrl())
		);
}
