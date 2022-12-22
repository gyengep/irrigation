#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/TemperatureHistorySamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureHistoryWriterTest, save) {
	for (const auto& temperatureHistorySample : DtoReaderWriterTestSamples::TemperatureHistorySampleList()) {
		const std::string actualXml = XmlWriter(false).save(temperatureHistorySample.getDto(), "MyStyleSheetFile");
		const std::string expectedXml = temperatureHistorySample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlAndStyleSheetHeader(expectedXml, "MyStyleSheetFile")));
	}
}
