#include <list>
#include <string>
#include "XmlReaderWriterTest.h"
#include "HotWeatherSchedulerSamples.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void testHotWeatherSchedulerRead(const HotWeatherSchedulerSample& hotWeatherSchedulerSample, XmlReader& reader) {
	const HotWeatherSchedulerDTO actualDto = reader.loadHotWeatherScheduler(hotWeatherSchedulerSample.first);
	EXPECT_THAT(actualDto, Eq(hotWeatherSchedulerSample.second));
}

void testHotWeatherSchedulerWrite(const HotWeatherSchedulerSample& hotWeatherSchedulerSample, XmlWriter& writer) {
	const string actualXml = writer.save(hotWeatherSchedulerSample.second);
	EXPECT_THAT(remove_xml_tag(actualXml), Eq(hotWeatherSchedulerSample.first));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(HotWeatherSchedulerReaderTest, schedulerInvalidType) {
	EXPECT_THROW(reader.loadHotWeatherScheduler("<scheduler type=\"ABC\"/>"), invalid_argument);
}

TEST_F(HotWeatherSchedulerReaderTest, schedulerInvalid) {
	EXPECT_THROW(reader.loadHotWeatherScheduler("<ABCD/>"), RequiredTagMissing);
}

TEST_F(HotWeatherSchedulerReaderTest, schedulerNoneXml) {
	EXPECT_THROW(reader.loadHotWeatherScheduler("jhvjhvjh"), XMLParseException);
}

TEST_F(HotWeatherSchedulerReaderTest, schedulerInvalidXml) {
	EXPECT_THROW(reader.loadHotWeatherScheduler("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(HotWeatherSchedulerReaderTest, schedulerAll) {
	testHotWeatherSchedulerRead(hotWeatherSchedulerSample_all, reader);
}

TEST_F(HotWeatherSchedulerReaderTest, schedulerPeriod) {
	testHotWeatherSchedulerRead(hotWeatherSchedulerSample_period, reader);
}

TEST_F(HotWeatherSchedulerReaderTest, schedulerTemperature) {
	testHotWeatherSchedulerRead(hotWeatherSchedulerSample_temperature, reader);
}

TEST_F(HotWeatherSchedulerReaderTest, schedulerEmpty) {
	testHotWeatherSchedulerRead(hotWeatherSchedulerSample_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(HotWeatherSchedulerWriterTest, schedulerAll) {
	testHotWeatherSchedulerWrite(hotWeatherSchedulerSample_all, writer);
}

TEST_F(HotWeatherSchedulerWriterTest, schedulerPeriod) {
	testHotWeatherSchedulerWrite(hotWeatherSchedulerSample_period, writer);
}

TEST_F(HotWeatherSchedulerWriterTest, schedulerTemperature) {
	testHotWeatherSchedulerWrite(hotWeatherSchedulerSample_temperature, writer);
}

TEST_F(HotWeatherSchedulerWriterTest, schedulerEmpty) {
	testHotWeatherSchedulerWrite(hotWeatherSchedulerSample_empty, writer);
}
