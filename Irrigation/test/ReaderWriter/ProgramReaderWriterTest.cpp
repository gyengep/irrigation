#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;


typedef std::pair<string, ProgramDTO> TestDataType;

///////////////////////////////////////////////////////////////////////////////

const TestDataType testData_all(
		"<program>"
			"<name>abcdefg</name>"
			"<schedulertype>specified</schedulertype>"
			"<schedulers>"
				"<scheduler type=\"specified\">"
					"<adjustment>110</adjustment>"
					"<days>"
						"<day>true</day>"
						"<day>false</day>"
					"</days>"
				"</scheduler>"
			"</schedulers>"
			"<runtimes>"
				"<runtime id=\"15\">20</runtime>"
				"<runtime id=\"25\">10</runtime>"
			"</runtimes>"
			"<starttimes>"
				"<starttime id=\"35\">50</starttime>"
				"<starttime id=\"45\">30</starttime>"
				"<starttime id=\"55\">100</starttime>"
			"</starttimes>"
		"</program>",
		ProgramDTO()
		.setName("abcdefg")
		.setSchedulerType("specified")
		.setSpecifiedScheduler(SpecifiedSchedulerDTO(110, new list<bool>({ true, false})))
		.setRunTimes(new list<RunTimeDTO>({
			RunTimeDTO(20).setId(15),
			RunTimeDTO(10).setId(25)}))
		.setStartTimes(new list<StartTimeDTO>({
			StartTimeDTO(50).setId(35),
			StartTimeDTO(30).setId(45),
			StartTimeDTO(100).setId(55)}))
		);

const TestDataType testData_name(
		"<program>"
			"<name>abcdefg</name>"
		"</program>",
		ProgramDTO().setName("abcdefg")
		);

const TestDataType testData_schedulerType(
		"<program>"
			"<schedulertype>specified</schedulertype>"
		"</program>",
		ProgramDTO().setSchedulerType("specified")
		);

const TestDataType testData_schedulers(
		"<program>"
			"<schedulers>"
				"<scheduler type=\"specified\">"
					"<adjustment>120</adjustment>"
					"<days>"
						"<day>true</day>"
						"<day>false</day>"
					"</days>"
				"</scheduler>"
			"</schedulers>"
		"</program>",
		ProgramDTO()
		.setSpecifiedScheduler(SpecifiedSchedulerDTO(120, new list<bool>({ true, false})))
		);

const TestDataType testData_runTimes(
		"<program>"
			"<runtimes>"
				"<runtime id=\"15\">20</runtime>"
				"<runtime id=\"25\">10</runtime>"
			"</runtimes>"
		"</program>",
		ProgramDTO()
		.setRunTimes(new list<RunTimeDTO>({
			RunTimeDTO(20).setId(15),
			RunTimeDTO(10).setId(25)}))
		);

const TestDataType testData_runTimesWithoutId(
		"<program>"
			"<runtimes>"
				"<runtime>20</runtime>"
				"<runtime>10</runtime>"
			"</runtimes>"
		"</program>",
		ProgramDTO()
		.setRunTimes(new list<RunTimeDTO>({
			RunTimeDTO(20),
			RunTimeDTO(10)}))
		);

const TestDataType testData_startTimes(
		"<program>"
			"<starttimes>"
				"<starttime id=\"35\">50</starttime>"
				"<starttime id=\"45\">30</starttime>"
				"<starttime id=\"55\">100</starttime>"
			"</starttimes>"
		"</program>",
		ProgramDTO()
		.setStartTimes(new list<StartTimeDTO>({
			StartTimeDTO(50).setId(35),
			StartTimeDTO(30).setId(45),
			StartTimeDTO(100).setId(55)}))
		);

const TestDataType testData_startTimesWithoutId(
		"<program>"
			"<starttimes>"
				"<starttime>50</starttime>"
				"<starttime>30</starttime>"
				"<starttime>100</starttime>"
			"</starttimes>"
		"</program>",
		ProgramDTO()
		.setStartTimes(new list<StartTimeDTO>({
			StartTimeDTO(50),
			StartTimeDTO(30),
			StartTimeDTO(100)}))
		);

const TestDataType testData_empty(
		"<program/>",
		ProgramDTO()
		);

///////////////////////////////////////////////////////////////////////////////

void testProgramRead(const TestDataType& testData, XmlReader& reader) {
	const ProgramDTO actualDto = reader.loadProgram(testData.first);
	EXPECT_EQ(testData.second, actualDto);
}

void testProgramWrite(const TestDataType& testData, XmlWriter& writer) {
	const string actualXml = writer.save(testData.second, false);
	EXPECT_EQ(testData.first, remove_xml_tag(actualXml));
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

TEST_F(ProgramReaderTest, programRunTimeWithoutId) {
	testProgramRead(testData_runTimesWithoutId, reader);
}

TEST_F(ProgramReaderTest, programStartTimeWithoutId) {
	testProgramRead(testData_startTimesWithoutId, reader);
}

TEST_F(ProgramReaderTest, programAll) {
	testProgramRead(testData_all, reader);
}

TEST_F(ProgramReaderTest, programName) {
	testProgramRead(testData_name, reader);
}

TEST_F(ProgramReaderTest, programSchedulerType) {
	testProgramRead(testData_schedulerType, reader);
}

TEST_F(ProgramReaderTest, programSchedulers) {
	testProgramRead(testData_schedulers, reader);
}

TEST_F(ProgramReaderTest, programRunTimes) {
	testProgramRead(testData_runTimes, reader);
}

TEST_F(ProgramReaderTest, programStartTimes) {
	testProgramRead(testData_startTimes, reader);
}

TEST_F(ProgramReaderTest, programEmpty) {
	testProgramRead(testData_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramWriterTest, programAll) {
	testProgramWrite(testData_all, writer);
}

TEST_F(ProgramWriterTest, programName) {
	testProgramWrite(testData_name, writer);
}

TEST_F(ProgramWriterTest, programSchedulerType) {
	testProgramWrite(testData_schedulerType, writer);
}

TEST_F(ProgramWriterTest, programSchedulers) {
	testProgramWrite(testData_schedulers, writer);
}

TEST_F(ProgramWriterTest, programRunTimes) {
	testProgramWrite(testData_runTimes, writer);
}

TEST_F(ProgramWriterTest, programStartTimes) {
	testProgramWrite(testData_startTimes, writer);
}

TEST_F(ProgramWriterTest, programEmpty) {
	testProgramWrite(testData_empty, writer);
}
