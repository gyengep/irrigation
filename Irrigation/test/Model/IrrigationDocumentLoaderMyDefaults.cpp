#include "Model/IrrigationDocumentLoaderMyDefaults.h"
#include "Mocks/MockIrrigationDocument.h"

using namespace testing;
using namespace std::placeholders;


TEST(IrrigationDocumentLoaderMyDefaultsTest, load) {
	MockIrrigationDocument mockIrrigationDocument;

	EXPECT_CALL(mockIrrigationDocument, updateFromDocumentDto(_)).
			Times(1);

	EXPECT_FALSE(IrrigationDocumentLoaderMyDefaults().load(mockIrrigationDocument));
}
