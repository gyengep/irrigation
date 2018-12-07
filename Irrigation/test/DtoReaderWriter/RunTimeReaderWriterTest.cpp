#include <list>
#include <string>
#include "XmlReaderWriterTest.h"
#include "RunTimeSamples.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void testRunTimeRead(const RunTimeSample& runTimeSample, XmlReader& reader) {
	const RunTimeDTO actualDto = reader.loadRunTime(runTimeSample.first);
	EXPECT_EQ(runTimeSample.second, actualDto);
}

void testRunTimeWrite(const RunTimeSample& runTimeSample, XmlWriter& writer) {
	const string actualXml = writer.save(runTimeSample.second);
	EXPECT_EQ(runTimeSample.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeReaderTest, runTimeInvalid) {
	EXPECT_THROW(reader.loadRunTime("<runtimeABC/>"), RequiredTagMissing);
}

TEST_F(RunTimeReaderTest, runTimeNoneXml) {
	EXPECT_THROW(reader.loadRunTime("jhvjhvjh"), XMLParseException);
}

TEST_F(RunTimeReaderTest, runTimeInvalidXml) {
	EXPECT_THROW(reader.loadRunTime("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeReaderTest, runTimeAll) {
	testRunTimeRead(runTimeSample_all, reader);
}

TEST_F(RunTimeReaderTest, runTimeMinute) {
	testRunTimeRead(runTimeSample_minute, reader);
}

TEST_F(RunTimeReaderTest, runTimeSecond) {
	testRunTimeRead(runTimeSample_second, reader);
}

TEST_F(RunTimeReaderTest, runTimeId) {
	testRunTimeRead(runTimeSample_id, reader);
}

TEST_F(RunTimeReaderTest, runTimeEmpty) {
	testRunTimeRead(runTimeSample_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeWriterTest, runTimeAll) {
	testRunTimeWrite(runTimeSample_all, writer);
}

TEST_F(RunTimeWriterTest, runTimeMinute) {
	testRunTimeWrite(runTimeSample_minute, writer);
}

TEST_F(RunTimeWriterTest, runTimeSecond) {
	testRunTimeWrite(runTimeSample_second, writer);
}

TEST_F(RunTimeWriterTest, runTimeId) {
	testRunTimeWrite(runTimeSample_id, writer);
}

TEST_F(RunTimeWriterTest, runTimeEmpty) {
	testRunTimeWrite(runTimeSample_empty, writer);
}
