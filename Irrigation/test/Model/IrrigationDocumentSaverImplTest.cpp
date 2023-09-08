#include "IrrigationDocumentSaverImplTest.h"
#include "Mocks/MockIrrigationDocument.h"

using namespace testing;
using namespace std::placeholders;


void IrrigationDocumentSaverImplTest::SetUp() {

	mockDtoWriter = std::make_shared<StrictMock<MockDtoWriter>>();
	mockFileWriter = std::make_shared<StrictMock<MockFileWriter>>();
	mockFileWriterFactory = std::make_shared<StrictMock<MockFileWriterFactory>>();

	irrigationDocumentSaverImpl = std::make_shared<IrrigationDocumentSaverImpl>(
			mockDtoWriter,
			mockFileWriterFactory
		);
}

void IrrigationDocumentSaverImplTest::TearDown() {

}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentSaverImplTest, save) {

	const DocumentDTO documentDto(
			std::list<ProgramDTO> {
				ProgramDTO().setName("111"),
				ProgramDTO().setName("222"),
				ProgramDTO().setName("333"),
			}
		);

	const std::string documentDtoAsText = "123456789";

	EXPECT_CALL(*mockDtoWriter, save(documentDto)).
			Times(1).
			WillOnce(Return(documentDtoAsText));

	EXPECT_CALL(*mockFileWriterFactory, create(FileWriter::Type::TRUNCATE)).
			Times(1).
			WillOnce(Return(mockFileWriter));

	EXPECT_CALL(*mockFileWriter, write(documentDtoAsText)).
			Times(1);

	MockIrrigationDocument mockIrrigationDocument;

	EXPECT_CALL(mockIrrigationDocument, toDocumentDto()).
			Times(1).
			WillRepeatedly(Return(documentDto));

	irrigationDocumentSaverImpl->save(mockIrrigationDocument);
}

