#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/TemperatureDependentSchedulerSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureDependentSchedulerWriterTest, save) {
	for (const auto& temperatureDependentSchedulerSample : DtoReaderWriterTestSamples::TemperatureDependentSchedulerSampleList()) {
		const std::string actualXml = XmlWriter(false).save(temperatureDependentSchedulerSample.getDto());
		const std::string expectedXml = temperatureDependentSchedulerSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlHeader(expectedXml)));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureDependentSchedulerReaderTest, load) {
	for (const auto& temperatureDependentSchedulerSample : DtoReaderWriterTestSamples::TemperatureDependentSchedulerSampleList()) {
		const TemperatureDependentSchedulerDTO actualDto = XmlReader().loadTemperatureDependentScheduler(temperatureDependentSchedulerSample.getXml());
		const TemperatureDependentSchedulerDTO expectedDto = temperatureDependentSchedulerSample.getDto();

		std::cout << actualDto << std::endl;

		EXPECT_THAT(actualDto, Eq(expectedDto));
	}
}

TEST(TemperatureDependentSchedulerReaderTest, loadInvalidType) {
	EXPECT_THROW(XmlReader().loadTemperatureDependentScheduler("<scheduler type=\"ABC\"/>"), std::invalid_argument);
}

TEST(TemperatureDependentSchedulerReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadTemperatureDependentScheduler("<hotweatherABC/>"), RequiredTagMissing);
}

TEST(TemperatureDependentSchedulerReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadTemperatureDependentScheduler("jhvjhvjh"), XMLParseException);
}

TEST(TemperatureDependentSchedulerReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadTemperatureDependentScheduler("<abc/><123/>"), XMLParseException);
}
