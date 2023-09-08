#include "IrrigationDocumentLoaderImplTest.h"
#include "Mocks/MockIrrigationDocument.h"

using namespace testing;
using namespace std::placeholders;


void IrrigationDocumentLoaderImplTest::SetUp() {

	mockDtoReader = std::make_shared<StrictMock<MockDtoReader>>();
	mockFileReader = std::make_shared<StrictMock<MockFileReader>>();

	irrigationDocumentLoaderImpl = std::make_shared<IrrigationDocumentLoaderImpl>(
			mockDtoReader,
			mockFileReader
		);
}

void IrrigationDocumentLoaderImplTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentLoaderImplTest, load) {
	const std::string documentDtoAsText = "asdfghjkl";
	const DocumentDTO expectedDocumentDto(
			std::list<ProgramDTO> {
				ProgramDTO().setName("111"),
				ProgramDTO().setName("222"),
				ProgramDTO().setName("333"),
			}
		);

	EXPECT_CALL(*mockFileReader, read()).
			Times(1).
			WillOnce(Return(documentDtoAsText));

	EXPECT_CALL(*mockDtoReader, loadDocument(documentDtoAsText)).
			Times(1).
			WillOnce(Return(expectedDocumentDto));

	MockIrrigationDocument mockIrrigationDocument;

	EXPECT_CALL(mockIrrigationDocument, updateFromDocumentDto(expectedDocumentDto)).
			Times(1);

	EXPECT_TRUE(irrigationDocumentLoaderImpl->load(mockIrrigationDocument));
}
