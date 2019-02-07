#include <list>
#include <string>
#include "XmlReaderWriterTest.h"
#include "StartTimeListSamples.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void testStartTimeListRead(const StartTimeListSample& startTimeListSample, XmlReader& reader) {
	const list<StartTimeDTO> actualDto = reader.loadStartTimeList(startTimeListSample.first);
	EXPECT_THAT(startTimeListSample.second, ContainerEq(actualDto));
}

void testStartTimeListWrite(const StartTimeListSample& startTimeListSample, XmlWriter& writer) {
	const string actualXml = writer.save(startTimeListSample.second);
	EXPECT_THAT(startTimeListSample.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeListReaderTest, startTimeListInvalid) {
	EXPECT_THROW(reader.loadStartTimeList("<starttimeABC/>"), RequiredTagMissing);
}

TEST_F(StartTimeListReaderTest, startTimeListNoneXml) {
	EXPECT_THROW(reader.loadStartTimeList("jhvjhvjh"), XMLParseException);
}

TEST_F(StartTimeListReaderTest, startTimeListInvalidXml) {
	EXPECT_THROW(reader.loadStartTimeList("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeListReaderTest, startTimeListMore) {
	testStartTimeListRead(startTimeListSample_more, reader);
}

TEST_F(StartTimeListReaderTest, startTimeListOne) {
	testStartTimeListRead(startTimeListSample_one, reader);
}

TEST_F(StartTimeListReaderTest, startTimeListEmpty) {
	testStartTimeListRead(startTimeListSample_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeListWriterTest, startTimeMore) {
	testStartTimeListWrite(startTimeListSample_more, writer);
}

TEST_F(StartTimeListWriterTest, startTimeOne) {
	testStartTimeListWrite(startTimeListSample_one, writer);
}

TEST_F(StartTimeListWriterTest, startTimeEmpty) {
	testStartTimeListWrite(startTimeListSample_empty, writer);
}
