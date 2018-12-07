#include <list>
#include <string>
#include "XmlReaderWriterTest.h"
#include "WeeklySchedulerSamples.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void testWeeklySchedulerRead(const WeeklySchedulerSample& weeklySchedulerSample, XmlReader& reader) {
	const WeeklySchedulerDTO actualDto = reader.loadWeeklyScheduler(weeklySchedulerSample.first);
	EXPECT_EQ(weeklySchedulerSample.second, actualDto);
}

void testWeeklySchedulerWrite(const WeeklySchedulerSample& weeklySchedulerSample, XmlWriter& writer) {
	const string actualXml = writer.save(weeklySchedulerSample.second);
	EXPECT_EQ(weeklySchedulerSample.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WeeklySchedulerReaderTest, schedulerInvalidType) {
	EXPECT_THROW(reader.loadWeeklyScheduler("<scheduler type=\"ABC\"/>"), invalid_argument);
}

TEST_F(WeeklySchedulerReaderTest, schedulerInvalid) {
	EXPECT_THROW(reader.loadWeeklyScheduler("<ABCD/>"), RequiredTagMissing);
}

TEST_F(WeeklySchedulerReaderTest, schedulerNoneXml) {
	EXPECT_THROW(reader.loadWeeklyScheduler("jhvjhvjh"), XMLParseException);
}

TEST_F(WeeklySchedulerReaderTest, schedulerInvalidXml) {
	EXPECT_THROW(reader.loadWeeklyScheduler("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WeeklySchedulerReaderTest, scheduler_all) {
	testWeeklySchedulerRead(weeklySchedulerSample_all, reader);
}

TEST_F(WeeklySchedulerReaderTest, schedulerAdjustment) {
	testWeeklySchedulerRead(weeklySchedulerSample_adjustment, reader);
}

TEST_F(WeeklySchedulerReaderTest, schedulerValues) {
	testWeeklySchedulerRead(weeklySchedulerSample_values, reader);
}

TEST_F(WeeklySchedulerReaderTest, schedulerEmpty) {
	testWeeklySchedulerRead(weeklySchedulerSample_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WeeklySchedulerWriterTest, scheduler_all) {
	testWeeklySchedulerWrite(weeklySchedulerSample_all, writer);
}

TEST_F(WeeklySchedulerWriterTest, schedulerAdjustment) {
	testWeeklySchedulerWrite(weeklySchedulerSample_adjustment, writer);
}

TEST_F(WeeklySchedulerWriterTest, schedulerValues) {
	testWeeklySchedulerWrite(weeklySchedulerSample_values, writer);
}

TEST_F(WeeklySchedulerWriterTest, schedulerEmpty) {
	testWeeklySchedulerWrite(weeklySchedulerSample_empty, writer);
}
