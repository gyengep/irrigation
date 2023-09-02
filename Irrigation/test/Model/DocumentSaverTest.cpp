#include "DocumentSaverTest.h"
#include "DTO/DocumentDTO.h"
#include "Model/DocumentSaver.h"
#include "Model/IrrigationDocumentImpl.h"
#include "Mocks/MockDtoReader.h"
#include "Mocks/MockFileReader.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockWateringController.h"

using namespace testing;
using namespace std::placeholders;


void DocumentSaverTest::SetUp() {

	mockProgramContainer = std::make_shared<StrictMock<MockProgramContainer>>();

	irrigationDocument = std::make_shared<IrrigationDocumentImpl>(
			mockProgramContainer,
			std::make_shared<MockWateringController>(),
			nullptr
		);

	mockDtoWriterFactory = std::make_shared<StrictMock<MockDtoWriterFactory>>(std::make_shared<StrictMock<MockDtoWriter>>());
	mockFileWriter = std::make_shared<StrictMock<MockFileWriter>>();
	mockFileWriterFactory = std::make_shared<StrictMock<MockFileWriterFactory>>();

	documentSaver = std::make_shared<DocumentSaver>(
		irrigationDocument,
		mockDtoWriterFactory,
		mockFileWriterFactory
	);
}

void DocumentSaverTest::TearDown() {

}

///////////////////////////////////////////////////////////////////////////////

TEST_F(DocumentSaverTest, isModifiedAfterLoad) {
	irrigationDocument->setModified();

	const std::string documentDtoAsText = "asdfghjkl";
	const DocumentDTO expectedDocumentDto;

	const std::shared_ptr<MockDtoReader> mockDtoReader = std::make_shared<MockDtoReader>();
	const std::shared_ptr<MockFileReader> mockFileReader = std::make_shared<MockFileReader>();

	EXPECT_CALL(*mockFileReader, read())
			.WillOnce(Return(documentDtoAsText));

	EXPECT_CALL(*mockDtoReader, loadDocument(documentDtoAsText))
			.WillOnce(Return(expectedDocumentDto));

	irrigationDocument->setModified(true);
	documentSaver->load(mockDtoReader, mockFileReader);

	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(DocumentSaverTest, load) {
	const std::string documentDtoAsText = "asdfghjkl";
	const DocumentDTO expectedDocumentDto;

	const std::shared_ptr<MockDtoReader> mockDtoReader = std::make_shared<MockDtoReader>();
	const std::shared_ptr<MockFileReader> mockFileReader = std::make_shared<MockFileReader>();

	EXPECT_CALL(*mockFileReader, read())
			.Times(1)
			.WillOnce(Return(documentDtoAsText));

	EXPECT_CALL(*mockDtoReader, loadDocument(documentDtoAsText))
			.Times(1)
			.WillOnce(Return(expectedDocumentDto));

	documentSaver->load(mockDtoReader, mockFileReader);
}

TEST_F(DocumentSaverTest, save) {

	const DocumentDTO documentDto(std::list<ProgramDTO>{
		ProgramDTO(),
		ProgramDTO(),
		ProgramDTO()
	});

	const std::string documentDtoAsText = "123456789";

	irrigationDocument->setModified(true);

	EXPECT_CALL(*mockProgramContainer, toProgramDtoList())
			.Times(1)
			.WillOnce(Return(documentDto.getPrograms()));

	EXPECT_CALL(*mockDtoWriterFactory->mockDtoWriter, save(documentDto))
			.Times(1)
			.WillOnce(Return(documentDtoAsText));

	EXPECT_CALL(*mockFileWriterFactory, create(FileWriter::Type::TRUNCATE))
			.Times(1)
			.WillOnce(Return(mockFileWriter));

	EXPECT_CALL(*mockFileWriter, write(documentDtoAsText))
			.Times(1);

	documentSaver->saveIfModified();
	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(DocumentSaverTest, saveNotModified) {
	irrigationDocument->setModified(false);
	documentSaver->saveIfModified();
}
