#include "XmlReaderWriterTest.h"
#include "Dto2Xml/CurrentTemperatureSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(CurrentTemperatureWriterTest, save) {
	for (const auto& currentTemperatureSample : Dto2XmlTest::CurrentTemperatureSampleList()) {
		const std::string actualXml = XmlWriter(false).save(currentTemperatureSample.getDto(), "MyStyleSheetFile");
		const std::string expectedXml = currentTemperatureSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlAndStyleSheetHeader(expectedXml, "MyStyleSheetFile")));
	}
}
