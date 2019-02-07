#include <list>
#include <string>
#include "XmlReaderWriterTest.h"
#include "ProgramListSamples.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void testProgramListRead(const ProgramListSample& programListSample, XmlReader& reader) {
	const list<ProgramDTO> actualDto = reader.loadProgramList(programListSample.first);
	EXPECT_THAT(programListSample.second, ContainerEq(actualDto));
}

void testProgramListWrite(const ProgramListSample& programListSample, XmlWriter& writer) {
	const string actualXml = writer.save(programListSample.second);
	EXPECT_THAT(programListSample.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramListReaderTest, programListInvalid) {
	EXPECT_THROW(reader.loadProgramList("<programABC/>"), RequiredTagMissing);
}

TEST_F(ProgramListReaderTest, programListNoneXml) {
	EXPECT_THROW(reader.loadProgramList("jhvjhvjh"), XMLParseException);
}

TEST_F(ProgramListReaderTest, programListInvalidXml) {
	EXPECT_THROW(reader.loadProgramList("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramListReaderTest, programListMore) {
	testProgramListRead(programListSample_more, reader);
}

TEST_F(ProgramListReaderTest, programListOne) {
	testProgramListRead(programListSample_one, reader);
}

TEST_F(ProgramListReaderTest, programListEmpty) {
	testProgramListRead(programListSample_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramListWriterTest, programMore) {
	testProgramListWrite(programListSample_more, writer);
}

TEST_F(ProgramListWriterTest, programOne) {
	testProgramListWrite(programListSample_one, writer);
}

TEST_F(ProgramListWriterTest, programEmpty) {
	testProgramListWrite(programListSample_empty, writer);
}
