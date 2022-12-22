#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/TemperatureForecastSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureForecastWriterTest, save) {
	for (const auto& temperatureForecastSample : DtoReaderWriterTestSamples::TemperatureForecastSampleList()) {
		const std::string actualXml = XmlWriter(false).save(temperatureForecastSample.getDto(), "MyStyleSheetFile");
		const std::string expectedXml = temperatureForecastSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlAndStyleSheetHeader(expectedXml, "MyStyleSheetFile")));
	}
}
