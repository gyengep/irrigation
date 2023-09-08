#include "Model/IrrigationDocumentImpl.h"
#include "Samples/DocumentSamples.h"
#include "Mocks/MockIrrigationDocumentLoader.h"
#include "Mocks/MockIrrigationDocumentSaver.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockWateringController.h"
#include <gmock/gmock.h>
#include <memory>

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

enum class DocumentDtoUpdateType {
	Nothing,
	ProgramContainer,
	All
};

void check(const DocumentDtoUpdateType updateType) {

	std::shared_ptr<MockProgramContainer> mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();

	IrrigationDocumentImpl actualDocument(
		std::make_shared<StrictMock<MockIrrigationDocumentLoader>>(),
		std::make_shared<StrictMock<MockIrrigationDocumentSaver>>(),
		mockProgramContainer,
		std::make_shared<NiceMock<MockWateringController>>(),
		nullptr
	);

	for (const auto& sample : Dto2ObjectTestSamples::DocumentSampleList()) {
		DocumentDTO actualDocumentDTO;

		if (DocumentDtoUpdateType::ProgramContainer == updateType || DocumentDtoUpdateType::All == updateType) {
			actualDocumentDTO.setPrograms(std::list<ProgramDTO>(sample.getDto().getPrograms()));
			EXPECT_CALL(*mockProgramContainer, updateFromProgramDtoList(sample.getObject().getProgramContainer().toProgramDtoList()));
		}

		actualDocument.updateFromDocumentDto(actualDocumentDTO);
	}
}

TEST(IrrigationDocumentImplFromDtoTest, updateFromDocumentDto_empty) {
	check(DocumentDtoUpdateType::Nothing);
}

TEST(IrrigationDocumentImplFromDtoTest, updateFromDocumentDto_partial_programContainer) {
	check(DocumentDtoUpdateType::ProgramContainer);
}

TEST(IrrigationDocumentImplFromDtoTest, updateFromDocumentDto_all) {
	check(DocumentDtoUpdateType::All);
}
