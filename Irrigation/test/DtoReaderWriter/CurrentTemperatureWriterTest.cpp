#include "XmlReaderWriterTest.h"
#include "Samples/CurrentTemperatureSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(CurrentTemperatureWriterTest, save) {
	for (const auto& currentTemperatureSample : DtoReaderWriterTestSamples::CurrentTemperatureSampleList()) {
		const std::string actualXml = XmlWriter(false).save(currentTemperatureSample.getDto(), "MyStyleSheetFile");
		const std::string expectedXml = currentTemperatureSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlAndStyleSheetHeader(expectedXml, "MyStyleSheetFile")));
	}
}
