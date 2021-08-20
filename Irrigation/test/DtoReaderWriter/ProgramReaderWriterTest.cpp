#include <list>
#include <string>
#include "XmlReaderWriterTest.h"
#include "ProgramSamples.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void testProgramRead(const ProgramSample& programSample, XmlReader& reader) {
	const ProgramDTO actualDto = reader.loadProgram(programSample.first);
	EXPECT_THAT(actualDto, Eq(programSample.second));
}

void testProgramWrite(const ProgramSample& programSample, XmlWriter& writer) {
	const string actualXml = writer.save(programSample.second);
	EXPECT_THAT(remove_xml_tag(actualXml), Eq(programSample.first));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramReaderTest, programInvalid) {
	EXPECT_THROW(reader.loadProgram("<prograsdcnnm/>"), RequiredTagMissing);
}

TEST_F(ProgramReaderTest, programNoneXml) {
	EXPECT_THROW(reader.loadProgram("jhvjhvjh"), XMLParseException);
}

TEST_F(ProgramReaderTest, programInvalidXml) {
	EXPECT_THROW(reader.loadProgram("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramReaderTest, programAll) {
	testProgramRead(programSample_all, reader);
}

TEST_F(ProgramReaderTest, programDisabled) {
	testProgramRead(programSample_disabled, reader);
}

TEST_F(ProgramReaderTest, programName) {
	testProgramRead(programSample_name, reader);
}

TEST_F(ProgramReaderTest, programAdjustment) {
	testProgramRead(programSample_adjustment, reader);
}

TEST_F(ProgramReaderTest, programSchedulerType) {
	testProgramRead(programSample_schedulerType, reader);
}

TEST_F(ProgramReaderTest, programWeeklyScheduler) {
	testProgramRead(programSample_weeklyScheduler, reader);
}

TEST_F(ProgramReaderTest, programRunTimes) {
	testProgramRead(programSample_runTimes, reader);
}

TEST_F(ProgramReaderTest, programStartTimes) {
	testProgramRead(programSample_startTimes, reader);
}

TEST_F(ProgramReaderTest, programId) {
	testProgramRead(programSample_id, reader);
}

TEST_F(ProgramReaderTest, programEmpty) {
	testProgramRead(programSample_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramWriterTest, programAll) {
	testProgramWrite(programSample_all, writer);
}

TEST_F(ProgramWriterTest, programDisabled) {
	testProgramWrite(programSample_disabled, writer);
}

TEST_F(ProgramWriterTest, programName) {
	testProgramWrite(programSample_name, writer);
}

TEST_F(ProgramWriterTest, programAdjustment) {
	testProgramWrite(programSample_adjustment, writer);
}

TEST_F(ProgramWriterTest, programSchedulerType) {
	testProgramWrite(programSample_schedulerType, writer);
}

TEST_F(ProgramWriterTest, programWeeklyScheduler) {
	testProgramWrite(programSample_weeklyScheduler, writer);
}

TEST_F(ProgramWriterTest, programRunTimes) {
	testProgramWrite(programSample_runTimes, writer);
}

TEST_F(ProgramWriterTest, programStartTimes) {
	testProgramWrite(programSample_startTimes, writer);
}

TEST_F(ProgramWriterTest, programId) {
	testProgramWrite(programSample_id, writer);
}

TEST_F(ProgramWriterTest, programEmpty) {
	testProgramWrite(programSample_empty, writer);
}
