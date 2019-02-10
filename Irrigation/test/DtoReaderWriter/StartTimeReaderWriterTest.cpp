#include <list>
#include <string>
#include "XmlReaderWriterTest.h"
#include "StartTimeSamples.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void testStartTimeRead(const StartTimeSample& startTimeSample, XmlReader& reader) {
	const StartTimeDTO actualDto = reader.loadStartTime(startTimeSample.first);
	EXPECT_THAT(actualDto, Eq(startTimeSample.second));
}

void testStartTimeWrite(const StartTimeSample& startTimeSample, XmlWriter& writer) {
	const string actualXml = writer.save(startTimeSample.second);
	EXPECT_THAT(remove_xml_tag(actualXml), Eq(startTimeSample.first));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeReaderTest, startTimeInvalid) {
	EXPECT_THROW(reader.loadStartTime("<starttime12345/>"), RequiredTagMissing);
}

TEST_F(StartTimeReaderTest, startTimeNoneXml) {
	EXPECT_THROW(reader.loadStartTime("jhvjhvjh"), XMLParseException);
}

TEST_F(StartTimeReaderTest, startTimeInvalidXml) {
	EXPECT_THROW(reader.loadStartTime("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeReaderTest, startTimeAll) {
	testStartTimeRead(startTimeSample_all, reader);
}

TEST_F(StartTimeReaderTest, startTimeHour) {
	testStartTimeRead(startTimeSample_hour, reader);
}

TEST_F(StartTimeReaderTest, startTimeMinute) {
	testStartTimeRead(startTimeSample_minute, reader);
}

TEST_F(StartTimeReaderTest, startTimeId) {
	testStartTimeRead(startTimeSample_id, reader);
}

TEST_F(StartTimeReaderTest, startTimeEmpty) {
	testStartTimeRead(startTimeSample_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeWriterTest, startTimeAll) {
	testStartTimeWrite(startTimeSample_all, writer);
}

TEST_F(StartTimeWriterTest, startTimeHour) {
	testStartTimeWrite(startTimeSample_hour, writer);
}

TEST_F(StartTimeWriterTest, startTimeMinute) {
	testStartTimeWrite(startTimeSample_minute, writer);
}

TEST_F(StartTimeWriterTest, startTimeId) {
	testStartTimeWrite(startTimeSample_id, writer);
}

TEST_F(StartTimeWriterTest, startTimeEmpty) {
	testStartTimeWrite(startTimeSample_empty, writer);
}
