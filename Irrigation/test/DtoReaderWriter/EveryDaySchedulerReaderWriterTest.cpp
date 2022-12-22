#include "XmlReaderWriterTest.h"
#include "Samples/EveryDaySchedulerSamples.h"
#include <list>
#include <string>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerWriterTest, save) {
	const DtoReaderWriterTestSamples::EveryDaySchedulerSample everyDaySchedulerSample;
	const std::string actualXml = XmlWriter(false).save(everyDaySchedulerSample.getDto());
	const std::string expectedXml = everyDaySchedulerSample.getXml();

	std::cout << actualXml << std::endl;

	EXPECT_THAT(remove_xml_tag(actualXml), Eq(expectedXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerReaderTest, load) {
	const DtoReaderWriterTestSamples::EveryDaySchedulerSample everyDaySchedulerSample;
	const EveryDaySchedulerDTO actualDto = XmlReader().loadEveryDayScheduler(everyDaySchedulerSample.getXml());
	const EveryDaySchedulerDTO expectedDto = everyDaySchedulerSample.getDto();

	std::cout << actualDto << std::endl;

	EXPECT_THAT(actualDto, Eq(expectedDto));
}

TEST(EveryDaySchedulerReaderTest, loadInvalidType) {
	EXPECT_THROW(XmlReader().loadEveryDayScheduler("<scheduler type=\"ABC\"/>"), std::invalid_argument);
}

TEST(EveryDaySchedulerReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadEveryDayScheduler("<hotweatherABC/>"), RequiredTagMissing);
}

TEST(EveryDaySchedulerReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadEveryDayScheduler("jhvjhvjh"), XMLParseException);
}

TEST(EveryDaySchedulerReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadEveryDayScheduler("<abc/><123/>"), XMLParseException);
}
