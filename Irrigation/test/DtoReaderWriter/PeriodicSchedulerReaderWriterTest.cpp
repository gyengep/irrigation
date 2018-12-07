#include <list>
#include <string>
#include "XmlReaderWriterTest.h"
#include "PeriodicSchedulerSamples.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void testPeriodicSchedulerRead(const PeriodicSchedulerSample& periodicSchedulerSample, XmlReader& reader) {
	const PeriodicSchedulerDTO actualDto = reader.loadPeriodicScheduler(periodicSchedulerSample.first);
	EXPECT_EQ(periodicSchedulerSample.second, actualDto);
}

void testPeriodicSchedulerWrite(const PeriodicSchedulerSample& periodicSchedulerSample, XmlWriter& writer) {
	const string actualXml = writer.save(periodicSchedulerSample.second);
	EXPECT_EQ(periodicSchedulerSample.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(PeriodicSchedulerReaderTest, schedulerInvalidType) {
	EXPECT_THROW(reader.loadPeriodicScheduler("<scheduler type=\"ABC\"/>"), invalid_argument);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerInvalid) {
	EXPECT_THROW(reader.loadPeriodicScheduler("<ABCD/>"), RequiredTagMissing);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerNoneXml) {
	EXPECT_THROW(reader.loadPeriodicScheduler("jhvjhvjh"), XMLParseException);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerInvalidXml) {
	EXPECT_THROW(reader.loadPeriodicScheduler("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(PeriodicSchedulerReaderTest, schedulerAll) {
	testPeriodicSchedulerRead(periodicSchedulerSample_all, reader);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerAdjustment) {
	testPeriodicSchedulerRead(periodicSchedulerSample_adjustment, reader);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerValues) {
	testPeriodicSchedulerRead(periodicSchedulerSample_values, reader);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerDate) {
	testPeriodicSchedulerRead(periodicSchedulerSample_date, reader);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerEmpty) {
	testPeriodicSchedulerRead(periodicSchedulerSample_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(PeriodicSchedulerWriterTest, schedulerAll) {
	testPeriodicSchedulerWrite(periodicSchedulerSample_all, writer);
}

TEST_F(PeriodicSchedulerWriterTest, schedulerAdjustment) {
	testPeriodicSchedulerWrite(periodicSchedulerSample_adjustment, writer);
}

TEST_F(PeriodicSchedulerWriterTest, schedulerValues) {
	testPeriodicSchedulerWrite(periodicSchedulerSample_values, writer);
}

TEST_F(PeriodicSchedulerWriterTest, schedulerDate) {
	testPeriodicSchedulerWrite(periodicSchedulerSample_date, writer);
}

TEST_F(PeriodicSchedulerWriterTest, schedulerEmpty) {
	testPeriodicSchedulerWrite(periodicSchedulerSample_empty, writer);
}
