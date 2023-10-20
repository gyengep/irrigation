#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/WeeklySchedulerSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerWriterTest, save) {
	for (const auto& weeklySchedulerSample : DtoReaderWriterTestSamples::WeeklySchedulerSampleList()) {
		const std::string actualXml = XmlWriter(false).save(weeklySchedulerSample.getDto());
		const std::string expectedXml = weeklySchedulerSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlHeader(expectedXml)));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerReaderTest, load) {
	for (const auto& weeklySchedulerSample : DtoReaderWriterTestSamples::WeeklySchedulerSampleList()) {
		const WeeklySchedulerDto actualDto = XmlReader().loadWeeklyScheduler(weeklySchedulerSample.getXml());
		const WeeklySchedulerDto expectedDto = weeklySchedulerSample.getDto();

		std::cout << actualDto << std::endl;

		EXPECT_THAT(actualDto, Eq(expectedDto));
	}
}

TEST(WeeklySchedulerReaderTest, loadInvalidType) {
	EXPECT_THROW(XmlReader().loadWeeklyScheduler("<scheduler type=\"ABC\"/>"), std::invalid_argument);
}

TEST(WeeklySchedulerReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadWeeklyScheduler("<hotweatherABC/>"), RequiredTagMissing);
}

TEST(WeeklySchedulerReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadWeeklyScheduler("jhvjhvjh"), XMLParseException);
}

TEST(WeeklySchedulerReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadWeeklyScheduler("<abc/><123/>"), XMLParseException);
}
