#include "XmlReaderWriterTest.h"
#include "Dto2Xml/TemperatureForecastSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureForecastWriterTest, save) {
	for (const auto& temperatureForecastSample : Dto2XmlTest::TemperatureForecastSampleList()) {
		const std::string actualXml = XmlWriter(false).save(temperatureForecastSample.getDto(), "MyStyleSheetFile");
		const std::string expectedXml = temperatureForecastSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlAndStyleSheetHeader(expectedXml, "MyStyleSheetFile")));
	}
}
