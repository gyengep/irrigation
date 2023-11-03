#include "IrrigationDocumentImplTest.h"
#include "Mocks/MockIrrigationDocumentLoader.h"
#include "Mocks/MockIrrigationDocumentSaver.h"
#include "Mocks/MockWateringController.h"
#include "DtoSamples/ProgramDtoSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const ProgramDtoList IrrigationDocumentImplDtoTest::sample(
		std::list<ProgramDto>{
			ProgramDto(DtoSamples::programDtoSample1).setId(48),
			ProgramDto(DtoSamples::programDtoSample2).setId(49),
			ProgramDto(DtoSamples::programDtoSample3).setId(50)
		}
	);

///////////////////////////////////////////////////////////////////////////////

void IrrigationDocumentImplDtoTest::SetUp() {
	IrrigationDocumentImplTest::SetUp();

	ON_CALL(*mockProgramContainer, toProgramDtoList()).WillByDefault(Return(sample));
}

void IrrigationDocumentImplDtoTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentImplDtoTest, toIrrigationDocumentDto) {
	const DocumentDto expectedDocumentDto(
			std::move(ProgramDtoList(sample))
		);

	EXPECT_CALL(*mockProgramContainer, toProgramDtoList()).Times(1);
	EXPECT_THAT(irrigationDocument->toDocumentDto(), Eq(expectedDocumentDto));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentImplDtoTest, updateFromDocumentDto_empty) {
	DocumentDto documentDto;

	irrigationDocument->updateFromDocumentDto(documentDto);
}

TEST_F(IrrigationDocumentImplDtoTest, updateFromDocumentDto_all) {
	const DocumentDto documentDto(
			std::move(ProgramDtoList(sample))
		);

	EXPECT_CALL(*mockProgramContainer, updateFromProgramDtoList(sample));

	irrigationDocument->updateFromDocumentDto(documentDto);
}
