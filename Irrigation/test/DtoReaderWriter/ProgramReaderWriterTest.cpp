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
				"<runtime id=\"15\"><minute>19</minute><second>23</second></runtime>"
				"<runtime id=\"25\"><minute>31</minute><second>46</second></runtime>"
			"</runtimes>"
			"<starttimes>"
				"<starttime id=\"35\"><hour>19</hour><minute>26</minute></starttime>"
				"<starttime id=\"45\"><hour>18</hour><minute>25</minute></starttime>"
				"<starttime id=\"55\"><hour>17</hour><minute>24</minute></starttime>"
			"</starttimes>"
		"</program>",
		ProgramDTO()
		.setName("abcdefg")
		.setSchedulerType("specified")
		.setSpecifiedScheduler(SpecifiedSchedulerDTO(110, new list<bool>({ true, false})))
		.setRunTimes(new list<RunTimeDTO>({
			RunTimeDTO(19, 23).setId(15),
			RunTimeDTO(31, 46).setId(25)}))
		.setStartTimes(new list<StartTimeDTO>({
			StartTimeDTO(19, 26).setId(35),
			StartTimeDTO(18, 25).setId(45),
			StartTimeDTO(17, 24).setId(55)}))
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
				"<runtime id=\"15\"><minute>3</minute><second>15</second></runtime>"
				"<runtime id=\"25\"><minute>7</minute><second>35</second></runtime>"
			"</runtimes>"
		"</program>",
		ProgramDTO()
		.setRunTimes(new list<RunTimeDTO>({
			RunTimeDTO(3, 15).setId(15),
			RunTimeDTO(7, 35).setId(25)}))
		);

const TestDataType testData_startTimes(
		"<program>"
			"<starttimes>"
				"<starttime id=\"35\"><hour>9</hour><minute>6</minute></starttime>"
				"<starttime id=\"45\"><hour>8</hour><minute>5</minute></starttime>"
				"<starttime id=\"55\"><hour>7</hour><minute>4</minute></starttime>"
			"</starttimes>"
		"</program>",
		ProgramDTO()
		.setStartTimes(new list<StartTimeDTO>({
			StartTimeDTO(9, 6).setId(35),
			StartTimeDTO(8, 5).setId(45),
			StartTimeDTO(7, 4).setId(55)}))
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
	const string actualXml = writer.save(testData.second);
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
	const TestDataType testData_runTimesWithoutId(
			"<program>"
				"<runtimes>"
					"<runtime><minute>3</minute><second>15</second></runtime>"
					"<runtime><minute>7</minute><second>35</second></runtime>"
				"</runtimes>"
			"</program>",
			ProgramDTO()
			.setRunTimes(new list<RunTimeDTO>({
				RunTimeDTO(3, 15),
				RunTimeDTO(7, 35)}))
			);

	testProgramRead(testData_runTimesWithoutId, reader);
}

TEST_F(ProgramReaderTest, programStartTimeWithoutId) {
	const TestDataType testData_startTimesWithoutId(
			"<program>"
				"<starttimes>"
					"<starttime><hour>9</hour><minute>6</minute></starttime>"
					"<starttime><hour>8</hour><minute>5</minute></starttime>"
					"<starttime><hour>7</hour><minute>4</minute></starttime>"
				"</starttimes>"
			"</program>",
			ProgramDTO()
			.setStartTimes(new list<StartTimeDTO>({
				StartTimeDTO(9, 6),
				StartTimeDTO(8, 5),
				StartTimeDTO(7, 4)}))
			);

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
