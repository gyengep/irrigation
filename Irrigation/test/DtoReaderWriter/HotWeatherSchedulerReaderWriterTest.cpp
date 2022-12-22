#include "XmlReaderWriterTest.h"
#include "Samples/HotWeatherSchedulerSamples.h"
#include <list>
#include <string>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerWriterTest, save) {
	for (const auto& hotWeatherSchedulerSample : DtoReaderWriterTestSamples::HotWeatherSchedulerSampleList()) {
		const std::string actualXml = XmlWriter(false).save(hotWeatherSchedulerSample.getDto());
		const std::string expectedXml = hotWeatherSchedulerSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(remove_xml_tag(actualXml), Eq(expectedXml));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerReaderTest, load) {
	for (const auto& hotWeatherSchedulerSample : DtoReaderWriterTestSamples::HotWeatherSchedulerSampleList()) {
		const HotWeatherSchedulerDTO actualDto = XmlReader().loadHotWeatherScheduler(hotWeatherSchedulerSample.getXml());
		const HotWeatherSchedulerDTO expectedDto = hotWeatherSchedulerSample.getDto();

		std::cout << actualDto << std::endl;

		EXPECT_THAT(actualDto, Eq(expectedDto));
	}
}

TEST(HotWeatherSchedulerReaderTest, loadInvalidType) {
	EXPECT_THROW(XmlReader().loadTemperatureDependentScheduler("<scheduler type=\"ABC\"/>"), std::invalid_argument);
}

TEST(HotWeatherSchedulerReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadHotWeatherScheduler("<hotweatherABC/>"), RequiredTagMissing);
}

TEST(HotWeatherSchedulerReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadHotWeatherScheduler("jhvjhvjh"), XMLParseException);
}

TEST(HotWeatherSchedulerReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadHotWeatherScheduler("<abc/><123/>"), XMLParseException);
}
