#include "DocumentSaverTest.h"
#include "Dto2Object/DocumentSamples.h"
#include "DTO/DocumentDto.h"
#include "Model/DocumentSaver.h"
#include "Model/IrrigationDocument.h"

using namespace std;
using namespace testing;
using namespace placeholders;


void DocumentSaverTest::SetUp() {
	irrigationDocument = IrrigationDocument::Builder().build();
	mockDtoWriterFactory = make_shared<MockDtoWriterFactory>(make_shared<MockDtoWriter>());
	mockFileWriterFactory = make_shared<MockFileWriterFactory>(make_shared<MockFileWriter>());

	documentSaver.reset(new DocumentSaver(
		irrigationDocument,
		mockDtoWriterFactory,
		mockFileWriterFactory
	));
}

void DocumentSaverTest::TearDown() {

}

///////////////////////////////////////////////////////////////////////////////

TEST_F(DocumentSaverTest, isModifiedAfterLoad) {
	irrigationDocument->setModified();

	const string documentDtoAsText = "asdfghjkl";
	const DocumentDTO expectedDocumentDto = Dto2ObjectTest::DocumentSample4().getDto();

	const shared_ptr<MockDtoReader> mockDtoReader = make_shared<MockDtoReader>();
	const shared_ptr<MockFileReader> mockFileReader = make_shared<MockFileReader>();

	EXPECT_CALL(*mockFileReader, read())
			.WillOnce(Return(documentDtoAsText));

	EXPECT_CALL(*mockDtoReader, loadDocument(documentDtoAsText))
			.WillOnce(Return(expectedDocumentDto));

	documentSaver->load(mockDtoReader, mockFileReader);

	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(DocumentSaverTest, isModifiedAfterSave) {
	irrigationDocument->setModified();
	documentSaver->saveIfModified();

	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(DocumentSaverTest, load) {
	const string documentDtoAsText = "asdfghjkl";
	const DocumentDTO expectedDocumentDto = Dto2ObjectTest::DocumentSample4().getDto();

	const shared_ptr<MockDtoReader> mockDtoReader = make_shared<MockDtoReader>();
	const shared_ptr<MockFileReader> mockFileReader = make_shared<MockFileReader>();

	EXPECT_CALL(*mockFileReader, read())
			.Times(1)
			.WillOnce(Return(documentDtoAsText));

	EXPECT_CALL(*mockDtoReader, loadDocument(documentDtoAsText))
			.Times(1)
			.WillOnce(Return(expectedDocumentDto));

	documentSaver->load(mockDtoReader, mockFileReader);
}

TEST_F(DocumentSaverTest, save) {
	const string documentDtoAsText = "123456789";
	const DocumentDTO documentDto = Dto2ObjectTest::DocumentSample4().getDto();

	irrigationDocument->updateFromDocumentDto(Dto2ObjectTest::DocumentSample4().getDto());
	irrigationDocument->setModified(true);

	EXPECT_CALL(*mockDtoWriterFactory->mockDtoWriter, save(documentDto))
			.Times(1)
			.WillOnce(Return(documentDtoAsText));

	EXPECT_CALL(*mockFileWriterFactory->mockFileWriter, write(documentDtoAsText))
			.Times(1);

	documentSaver->saveIfModified();
}

TEST_F(DocumentSaverTest, saveNotModified) {
	const DocumentDTO documentDto = Dto2ObjectTest::DocumentSample4().getDto();

	irrigationDocument->updateFromDocumentDto(Dto2ObjectTest::DocumentSample4().getDto());
	irrigationDocument->setModified(false);

	EXPECT_CALL(*mockDtoWriterFactory->mockDtoWriter, save(documentDto))
			.Times(0);

	EXPECT_CALL(*mockFileWriterFactory->mockFileWriter, write(_))
			.Times(0);

	documentSaver->saveIfModified();
}
