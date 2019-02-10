#include <list>
#include <string>
#include "XmlReaderWriterTest.h"
#include "RunTimeListSamples.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void testRunTimeListRead(const RunTimeListSample& runTimeListSample, XmlReader& reader) {
	const list<RunTimeDTO> actualDto = reader.loadRunTimeList(runTimeListSample.first);
	EXPECT_THAT(runTimeListSample.second, ContainerEq(actualDto));
}

void testRunTimeListWrite(const RunTimeListSample& runTimeListSample, XmlWriter& writer) {
	const string actualXml = writer.save(runTimeListSample.second);
	EXPECT_THAT(runTimeListSample.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeListReaderTest, runTimeListInvalid) {
	EXPECT_THROW(reader.loadRunTimeList("<runtimeABC/>"), RequiredTagMissing);
}

TEST_F(RunTimeListReaderTest, runTimeListNoneXml) {
	EXPECT_THROW(reader.loadRunTimeList("jhvjhvjh"), XMLParseException);
}

TEST_F(RunTimeListReaderTest, runTimeListInvalidXml) {
	EXPECT_THROW(reader.loadRunTimeList("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeListReaderTest, runTimeListMore) {
	testRunTimeListRead(runTimeListSample_more, reader);
}

TEST_F(RunTimeListReaderTest, runTimeListOne) {
	testRunTimeListRead(runTimeListSample_one, reader);
}

TEST_F(RunTimeListReaderTest, runTimeListEmpty) {
	testRunTimeListRead(runTimeListSample_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeListWriterTest, runTimeMore) {
	testRunTimeListWrite(runTimeListSample_more, writer);
}

TEST_F(RunTimeListWriterTest, runTimeOne) {
	testRunTimeListWrite(runTimeListSample_one, writer);
}

TEST_F(RunTimeListWriterTest, runTimeEmpty) {
	testRunTimeListWrite(runTimeListSample_empty, writer);
}
