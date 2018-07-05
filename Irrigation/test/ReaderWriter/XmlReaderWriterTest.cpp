#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;



static string remove_xml_tag(const string& text) {
	string copy(text);

	size_t pos = copy.find('>');
	if (string::npos == pos) {
		throw logic_error("check_xml");
	}

	copy.erase(0, pos + 1);
	return copy;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeReaderWriterTest, runTimeAll) {
	const string expectedXml = "<runtime id=\"1\">5</runtime>";
	const RunTimeDTO runTime = reader.loadRunTime(expectedXml);

	EXPECT_THAT(runTime, RunTimeDTO(5).setId(1));

	const string actualXml = writer.save(runTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(RunTimeReaderWriterTest, runTimeValue) {
	const string expectedXml = "<runtime>5</runtime>";
	const RunTimeDTO runTime = reader.loadRunTime(expectedXml);

	EXPECT_THAT(runTime, RunTimeDTO(5));

	const string actualXml = writer.save(runTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(RunTimeReaderWriterTest, runTimeId) {
	const string expectedXml = "<runtime id=\"1\"/>";
	const RunTimeDTO runTime = reader.loadRunTime(expectedXml);

	EXPECT_THAT(runTime, RunTimeDTO().setId(1));

	const string actualXml = writer.save(runTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(RunTimeReaderWriterTest, runTimeEmpty) {
	const string expectedXml = "<runtime/>";
	const RunTimeDTO runTime = reader.loadRunTime(expectedXml);

	EXPECT_THAT(runTime, RunTimeDTO());

	const string actualXml = writer.save(runTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(RunTimeReaderWriterTest, runTimeInvalid) {
	const string expectedXml = "<runtimeABC/>";

	EXPECT_THROW(reader.loadRunTime(expectedXml), RequiredTagMissing);
}

TEST_F(RunTimeReaderWriterTest, runTimeNoneXml) {
	EXPECT_THROW(reader.loadRunTime("jhvjhvjh"), XMLParseException);
}

TEST_F(RunTimeReaderWriterTest, runTimeInvalidXml) {
	EXPECT_THROW(reader.loadRunTime("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeReaderWriterTest, startTimeAll) {
	const string expectedXml = "<starttime id=\"1\">5</starttime>";
	const StartTimeDTO startTime = reader.loadStartTime(expectedXml);

	EXPECT_THAT(startTime, StartTimeDTO(5).setId(1));

	const string actualXml = writer.save(startTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(StartTimeReaderWriterTest, startTimeValue) {
	const string expectedXml = "<starttime>5</starttime>";
	const StartTimeDTO startTime = reader.loadStartTime(expectedXml);

	EXPECT_THAT(startTime, StartTimeDTO(5));

	const string actualXml = writer.save(startTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(StartTimeReaderWriterTest, startTimeId) {
	const string expectedXml = "<starttime id=\"1\"/>";
	const StartTimeDTO startTime = reader.loadStartTime(expectedXml);

	EXPECT_THAT(startTime, StartTimeDTO().setId(1));

	const string actualXml = writer.save(startTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(StartTimeReaderWriterTest, startTimeEmpty) {
	const string expectedXml = "<starttime/>";
	const StartTimeDTO startTime = reader.loadStartTime(expectedXml);

	EXPECT_THAT(startTime, StartTimeDTO());

	const string actualXml = writer.save(startTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(StartTimeReaderWriterTest, startTimeInvalid) {
	const string expectedXml = "<starttime12345/>";

	EXPECT_THROW(reader.loadStartTime(expectedXml), RequiredTagMissing);
}

TEST_F(StartTimeReaderWriterTest, startTimeNoneXml) {
	EXPECT_THROW(reader.loadStartTime("jhvjhvjh"), XMLParseException);
}

TEST_F(StartTimeReaderWriterTest, startTimeInvalidXml) {
	EXPECT_THROW(reader.loadStartTime("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerAll) {
	const string expectedXml =
			"<scheduler type=\"specified\">"
				"<adjustment>90</adjustment>"
				"<days>"
					"<day>true</day>"
					"<day>false</day>"
				"</days>"
			"</scheduler>";

	const SpecifiedSchedulerDTO scheduler = reader.loadSpecifiedScheduler(expectedXml);

	EXPECT_THAT(scheduler, SpecifiedSchedulerDTO(90, new list<bool>({ true, false })));

	const string actualXml = writer.save(scheduler, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerAdjustment) {
	const string expectedXml =
			"<scheduler type=\"specified\">"
				"<adjustment>80</adjustment>"
			"</scheduler>";

	const SpecifiedSchedulerDTO scheduler = reader.loadSpecifiedScheduler(expectedXml);

	EXPECT_THAT(scheduler, SpecifiedSchedulerDTO().setAdjustment(80));

	const string actualXml = writer.save(scheduler, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerValues) {
	const string expectedXml =
			"<scheduler type=\"specified\">"
				"<days>"
					"<day>true</day>"
					"<day>false</day>"
				"</days>"
			"</scheduler>";

	const SpecifiedSchedulerDTO scheduler = reader.loadSpecifiedScheduler(expectedXml);

	EXPECT_THAT(scheduler, SpecifiedSchedulerDTO().setValues(new list<bool>({ true, false })));

	const string actualXml = writer.save(scheduler, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerEmpty) {
	const string expectedXml = "<scheduler type=\"specified\"/>";
	const SpecifiedSchedulerDTO scheduler = reader.loadSpecifiedScheduler(expectedXml);

	EXPECT_THAT(scheduler, SpecifiedSchedulerDTO());

	const string actualXml = writer.save(scheduler, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerInvalidType) {
	const string expectedXml = "<scheduler type=\"ABC\"/>";

	EXPECT_THROW(reader.loadSpecifiedScheduler(expectedXml), invalid_argument);
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerInvalid) {
	const string expectedXml = "<ABCD/>";

	EXPECT_THROW(reader.loadSpecifiedScheduler(expectedXml), RequiredTagMissing);
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerNoneXml) {
	EXPECT_THROW(reader.loadSpecifiedScheduler("jhvjhvjh"), XMLParseException);
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerInvalidXml) {
	EXPECT_THROW(reader.loadSpecifiedScheduler("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST_F(ProgramReaderWriterTest, programAll) {
	const string expectedXml =
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
			"</program>";

	const ProgramDTO program = reader.loadProgram(expectedXml);

	EXPECT_THAT(program,
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
				StartTimeDTO(100).setId(55)})
			));

	const string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(ProgramReaderWriterTest, programName) {
	const string expectedXml =
			"<program>"
				"<name>abcdefg</name>"
			"</program>";

	const ProgramDTO program = reader.loadProgram(expectedXml);

	EXPECT_THAT(program, ProgramDTO().setName("abcdefg"));

	const string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(ProgramReaderWriterTest, programSchedulerType) {
	const string expectedXml =
			"<program>"
				"<schedulertype>specified</schedulertype>"
			"</program>";

	const ProgramDTO program = reader.loadProgram(expectedXml);

	EXPECT_THAT(program, ProgramDTO().setSchedulerType("specified"));

	const string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(ProgramReaderWriterTest, programSchedulers) {
	const string expectedXml =
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
			"</program>";

	const ProgramDTO program = reader.loadProgram(expectedXml);

	EXPECT_THAT(program,
			ProgramDTO()
			.setSpecifiedScheduler(SpecifiedSchedulerDTO(120, new list<bool>({ true, false})))
			);

	const string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(ProgramReaderWriterTest, programRunTimes) {
	const string expectedXml =
			"<program>"
				"<runtimes>"
					"<runtime id=\"15\">20</runtime>"
					"<runtime id=\"25\">10</runtime>"
				"</runtimes>"
			"</program>";

	const ProgramDTO program = reader.loadProgram(expectedXml);

	EXPECT_THAT(program,
			ProgramDTO()
			.setRunTimes(new list<RunTimeDTO>({
				RunTimeDTO(20).setId(15),
				RunTimeDTO(10).setId(25)}))
			);

	const string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(ProgramReaderWriterTest, programRunTimeIdMissing) {
	const string expectedXml =
			"<program>"
				"<runtimes>"
					"<runtime>50</runtime>"
				"</runtimes>"
			"</program>";

	EXPECT_THROW(reader.loadProgram(expectedXml), RequiredAttributeMissing);
}

TEST_F(ProgramReaderWriterTest, programStartTimes) {
	const string expectedXml =
			"<program>"
				"<starttimes>"
					"<starttime id=\"35\">50</starttime>"
					"<starttime id=\"45\">30</starttime>"
					"<starttime id=\"55\">100</starttime>"
				"</starttimes>"
			"</program>";

	const ProgramDTO program = reader.loadProgram(expectedXml);

	EXPECT_THAT(program,
			ProgramDTO()
			.setStartTimes(new list<StartTimeDTO>({
				StartTimeDTO(50).setId(35),
				StartTimeDTO(30).setId(45),
				StartTimeDTO(100).setId(55)}))
			);

	const string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(ProgramReaderWriterTest, programStartTimeIdMissing) {
	const string expectedXml =
			"<program>"
				"<starttimes>"
					"<starttime>50</starttime>"
				"</starttimes>"
			"</program>";

	EXPECT_THROW(reader.loadProgram(expectedXml), RequiredAttributeMissing);
}

TEST_F(ProgramReaderWriterTest, programEmpty) {
	const string expectedXml = "<program/>";
	const ProgramDTO program = reader.loadProgram(expectedXml);

	EXPECT_THAT(program, ProgramDTO());

	const string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(ProgramReaderWriterTest, programInvalid) {
	const string expectedXml = "<prograsdcnnm/>";

	EXPECT_THROW(reader.loadProgram(expectedXml), RequiredTagMissing);
}

TEST_F(ProgramReaderWriterTest, programNoneXml) {
	EXPECT_THROW(reader.loadProgram("jhvjhvjh"), XMLParseException);
}

TEST_F(ProgramReaderWriterTest, programInvalidXml) {
	EXPECT_THROW(reader.loadProgram("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST_F(DocumentReaderWriterTest, documentAll) {
	const string expectedXml =
			"<irrigation>"
				"<programs>"
					"<program id=\"5\">"
						"<name>abcdefg</name>"
						"<schedulertype>specified</schedulertype>"
						"<schedulers>"
							"<scheduler type=\"specified\">"
								"<adjustment>130</adjustment>"
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
					"</program>"
					"<program id=\"6\">"
						"<name>123456</name>"
						"<schedulertype>specified</schedulertype>"
						"<schedulers>"
							"<scheduler type=\"specified\">"
								"<adjustment>140</adjustment>"
								"<days>"
									"<day>true</day>"
									"<day>true</day>"
									"<day>false</day>"
									"<day>false</day>"
								"</days>"
							"</scheduler>"
						"</schedulers>"
						"<runtimes>"
							"<runtime id=\"115\">200</runtime>"
							"<runtime id=\"125\">210</runtime>"
							"<runtime id=\"135\">205</runtime>"
						"</runtimes>"
						"<starttimes>"
							"<starttime id=\"145\">130</starttime>"
							"<starttime id=\"155\">150</starttime>"
						"</starttimes>"
					"</program>"
				"</programs>"
			"</irrigation>";

	const DocumentDTO document = reader.loadDocument(expectedXml);

	EXPECT_THAT(document, DocumentDTO(
		new list<ProgramDTO>({
			ProgramDTO()
				.setId(5)
				.setName("abcdefg")
				.setSchedulerType("specified")
				.setSpecifiedScheduler(SpecifiedSchedulerDTO(130, new list<bool>({ true, false })))
				.setRunTimes(new list<RunTimeDTO>({
					RunTimeDTO(20).setId(15),
					RunTimeDTO(10).setId(25)}))
				.setStartTimes(new list<StartTimeDTO>({
					StartTimeDTO(50).setId(35),
					StartTimeDTO(30).setId(45),
					StartTimeDTO(100).setId(55)})),
			ProgramDTO()
				.setId(6)
				.setName("123456")
				.setSchedulerType("specified")
				.setSpecifiedScheduler(SpecifiedSchedulerDTO(140, new list<bool>({ true, true, false, false })))
				.setRunTimes(new list<RunTimeDTO>({
					RunTimeDTO(200).setId(115),
					RunTimeDTO(210).setId(125),
					RunTimeDTO(205).setId(135)}))
				.setStartTimes(new list<StartTimeDTO>({
					StartTimeDTO(130).setId(145),
					StartTimeDTO(150).setId(155)}))
			})));

	const string actualXml = writer.save(document, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(DocumentReaderWriterTest, ProgramIdMissing) {
	const string expectedXml =
			"<irrigation>"
				"<programs>"
					"<program/>"
				"</programs>"
			"</irrigation>";

	EXPECT_THROW(reader.loadDocument(expectedXml), RequiredAttributeMissing);
}

TEST_F(DocumentReaderWriterTest, documentEmpty) {
	const string expectedXml = "<irrigation/>";
	const DocumentDTO document = reader.loadDocument(expectedXml);

	EXPECT_THAT(document, DocumentDTO());

	const string actualXml = writer.save(document, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(DocumentReaderWriterTest, documentInvalid) {
	const string expectedXml = "<irrig/>";

	EXPECT_THROW(reader.loadDocument(expectedXml), RequiredTagMissing);
}

TEST_F(DocumentReaderWriterTest, documentNoneXml) {
	EXPECT_THROW(reader.loadDocument("jhvjhvjh"), XMLParseException);
}

TEST_F(DocumentReaderWriterTest, documentInvalidXml) {
	EXPECT_THROW(reader.loadDocument("<abc/><123/>"), XMLParseException);
}
