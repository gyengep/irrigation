#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/EveryDaySchedulerSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerWriterTest, save) {
	const DtoReaderWriterTestSamples::EveryDaySchedulerSample everyDaySchedulerSample;
	const std::string actualXml = XmlWriter(false).save(everyDaySchedulerSample.getDto());
	const std::string expectedXml = everyDaySchedulerSample.getXml();

	std::cout << actualXml << std::endl;

	EXPECT_THAT(actualXml, Eq(prependXmlHeader(expectedXml)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerReaderTest, load) {
	const DtoReaderWriterTestSamples::EveryDaySchedulerSample everyDaySchedulerSample;
	const EveryDaySchedulerDto actualDto = XmlReader().loadEveryDayScheduler(everyDaySchedulerSample.getXml());
	const EveryDaySchedulerDto expectedDto = everyDaySchedulerSample.getDto();

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
