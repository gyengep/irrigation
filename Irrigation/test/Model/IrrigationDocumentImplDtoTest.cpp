#include "IrrigationDocumentImplDtoTest.h"
#include "Dto2ObjectSamples/DocumentSamples.h"
#include "Mocks/MockIrrigationDocumentLoader.h"
#include "Mocks/MockIrrigationDocumentSaver.h"
#include "Mocks/MockWateringController.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(IrrigationDocumentImplToDtoTest, toDocumentDto) {
	const Dto2ObjectTestSamples::DocumentSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const IrrigationDocumentImpl& actual = sample.getObject();
		const DocumentDto& expected = sample.getDto();

		EXPECT_THAT(actual.toDocumentDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

void IrrigationDocumentImplFromDtoTest::SetUp() {
	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();

	irrigationDocument = std::make_shared<IrrigationDocumentImpl>(
			std::make_shared<StrictMock<MockIrrigationDocumentLoader>>(),
			std::make_shared<StrictMock<MockIrrigationDocumentSaver>>(),
			mockProgramContainer,
			std::make_shared<StrictMock<MockWateringController>>(),
			nullptr
		);
}

void IrrigationDocumentImplFromDtoTest::TearDown() {
}

void IrrigationDocumentImplFromDtoTest::check(const UpdateType updateType) {
	const auto sampleList = Dto2ObjectTestSamples::DocumentSampleList();

	for (const auto& sample : sampleList) {
		DocumentDto actualDocumentDto;

		if (UpdateType::ProgramContainer == updateType || UpdateType::All == updateType) {
			const std::list<ProgramDto> expectedProgramDtoList = sample.getDto().getPrograms();

			actualDocumentDto.setPrograms(std::list<ProgramDto>(expectedProgramDtoList));
			EXPECT_CALL(*mockProgramContainer, updateFromProgramDtoList(expectedProgramDtoList));
		}

		irrigationDocument->updateFromDocumentDto(actualDocumentDto);
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentImplFromDtoTest, updateFromDocumentDto_empty) {
	check(UpdateType::Nothing);
}

TEST_F(IrrigationDocumentImplFromDtoTest, updateFromDocumentDto_partial_programContainer) {
	check(UpdateType::ProgramContainer);
}

TEST_F(IrrigationDocumentImplFromDtoTest, updateFromDocumentDto_all) {
	check(UpdateType::All);
}
