#include "XmlReaderWriterTest.h"
#include "Dto2Xml/TemperatureHistorySamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureHistoryWriterTest, save) {
	for (const auto& temperatureHistorySample : Dto2XmlTest::TemperatureHistorySampleList()) {
		const std::string actualXml = XmlWriter(false).save(temperatureHistorySample.getDto(), "MyStyleSheetFile");
		const std::string expectedXml = temperatureHistorySample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlAndStyleSheetHeader(expectedXml, "MyStyleSheetFile")));
	}
}
