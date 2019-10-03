#include <list>
#include <string>
#include "XmlReaderWriterTest.h"
#include "TemperatureDependentSchedulerSamples.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void testTemperatureDependentSchedulerRead(const TemperatureDependentSchedulerSample& temperatureDependentSchedulerSample, XmlReader& reader) {
	const TemperatureDependentSchedulerDTO actualDto = reader.loadTemperatureDependentScheduler(temperatureDependentSchedulerSample.first);
	EXPECT_THAT(actualDto, Eq(temperatureDependentSchedulerSample.second));
}

void testTemperatureDependentSchedulerWrite(const TemperatureDependentSchedulerSample& temperatureDependentSchedulerSample, XmlWriter& writer) {
	const string actualXml = writer.save(temperatureDependentSchedulerSample.second);
	EXPECT_THAT(remove_xml_tag(actualXml), Eq(temperatureDependentSchedulerSample.first));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerInvalidType) {
	EXPECT_THROW(reader.loadTemperatureDependentScheduler("<scheduler type=\"ABC\"/>"), invalid_argument);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerInvalid) {
	EXPECT_THROW(reader.loadTemperatureDependentScheduler("<ABCD/>"), RequiredTagMissing);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerNoneXml) {
	EXPECT_THROW(reader.loadTemperatureDependentScheduler("jhvjhvjh"), XMLParseException);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerInvalidXml) {
	EXPECT_THROW(reader.loadTemperatureDependentScheduler("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerAll) {
	testTemperatureDependentSchedulerRead(temperatureDependentSchedulerSample_all, reader);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerRemainingA) {
	testTemperatureDependentSchedulerRead(temperatureDependentSchedulerSample_remainingA, reader);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerForecastA) {
	testTemperatureDependentSchedulerRead(temperatureDependentSchedulerSample_forecastA, reader);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerForecastB) {
	testTemperatureDependentSchedulerRead(temperatureDependentSchedulerSample_forecastB, reader);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerHistoryA) {
	testTemperatureDependentSchedulerRead(temperatureDependentSchedulerSample_historyA, reader);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerHistoryB) {
	testTemperatureDependentSchedulerRead(temperatureDependentSchedulerSample_historyA, reader);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerMinAdjustment) {
	testTemperatureDependentSchedulerRead(temperatureDependentSchedulerSample_minAdjustment, reader);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerMaxAdjustment) {
	testTemperatureDependentSchedulerRead(temperatureDependentSchedulerSample_maxAdjustment, reader);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerTrim) {
	testTemperatureDependentSchedulerRead(temperatureDependentSchedulerSample_trim, reader);
}

TEST_F(TemperatureDependentSchedulerReaderTest, schedulerEmpty) {
	testTemperatureDependentSchedulerRead(temperatureDependentSchedulerSample_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(TemperatureDependentSchedulerWriterTest, schedulerAll) {
	testTemperatureDependentSchedulerWrite(temperatureDependentSchedulerSample_all, writer);
}

TEST_F(TemperatureDependentSchedulerWriterTest, schedulerRemainingA) {
	testTemperatureDependentSchedulerWrite(temperatureDependentSchedulerSample_remainingA, writer);
}

TEST_F(TemperatureDependentSchedulerWriterTest, schedulerForecastA) {
	testTemperatureDependentSchedulerWrite(temperatureDependentSchedulerSample_forecastA, writer);
}

TEST_F(TemperatureDependentSchedulerWriterTest, schedulerForecastB) {
	testTemperatureDependentSchedulerWrite(temperatureDependentSchedulerSample_forecastB, writer);
}

TEST_F(TemperatureDependentSchedulerWriterTest, schedulerHistoryA) {
	testTemperatureDependentSchedulerWrite(temperatureDependentSchedulerSample_historyA, writer);
}

TEST_F(TemperatureDependentSchedulerWriterTest, schedulerHistoryB) {
	testTemperatureDependentSchedulerWrite(temperatureDependentSchedulerSample_historyB, writer);
}

TEST_F(TemperatureDependentSchedulerWriterTest, schedulerMinAdjustment) {
	testTemperatureDependentSchedulerWrite(temperatureDependentSchedulerSample_minAdjustment, writer);
}

TEST_F(TemperatureDependentSchedulerWriterTest, schedulerMaxAdjustment) {
	testTemperatureDependentSchedulerWrite(temperatureDependentSchedulerSample_maxAdjustment, writer);
}

TEST_F(TemperatureDependentSchedulerWriterTest, schedulerTrim) {
	testTemperatureDependentSchedulerWrite(temperatureDependentSchedulerSample_trim, writer);
}

TEST_F(TemperatureDependentSchedulerWriterTest, schedulerEmpty) {
	testTemperatureDependentSchedulerWrite(temperatureDependentSchedulerSample_empty, writer);
}
