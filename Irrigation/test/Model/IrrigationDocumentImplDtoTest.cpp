#include "IrrigationDocumentImplDtoTest.h"
#include "Dto2ObjectSamples/DocumentSamples.h"
#include "Mocks/MockIrrigationDocumentLoader.h"
#include "Mocks/MockIrrigationDocumentSaver.h"
#include "Mocks/MockWateringController.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(IrrigationDocumentImplToDtoTest, toIrrigationDocumentDto) {
	const Dto2ObjectTestSamples::DocumentSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const IrrigationDocumentImpl& actual = sample.getObject();
		const DocumentDTO& expected = sample.getDto();

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
		DocumentDTO actualDocumentDTO;

		if (UpdateType::ProgramContainer == updateType || UpdateType::All == updateType) {
			const std::list<ProgramDTO> expectedProgramDtoList = sample.getDto().getPrograms();

			actualDocumentDTO.setPrograms(std::list<ProgramDTO>(expectedProgramDtoList));
			EXPECT_CALL(*mockProgramContainer, updateFromProgramDtoList(expectedProgramDtoList));
		}

		irrigationDocument->updateFromDocumentDto(actualDocumentDTO);
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
