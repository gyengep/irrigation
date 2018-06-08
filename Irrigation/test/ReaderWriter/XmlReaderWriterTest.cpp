#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;



static std::string remove_xml_tag(const std::string& text) {
	string copy(text);

	size_t pos = copy.find('>');
	if (string::npos == pos) {
		throw invalid_argument("check_xml");
	}

	copy.erase(0, pos + 1);
	return copy;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST(XmlReader, loadNoneXml) {
	XmlReader reader;
	EXPECT_THROW(reader.loadFromString("jhvjhvjh"), XmlParsingError);
}


TEST(XmlReader, loadInvalidXml) {
	XmlReader reader;
	EXPECT_THROW(reader.loadFromString("<abc/><123/>"), XmlParsingError);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeReaderWriterTest, runTimeAll) {
	const string expectedXml = "<runtime id=\"1\">5</runtime>";

	reader.loadFromString(expectedXml);
	reader.load(runTime);

	EXPECT_THAT(runTime, RunTimeDTO(5).setId(1));

	writer.save(runTime);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(RunTimeReaderWriterTest, runTimeValue) {
	const string expectedXml = "<runtime>5</runtime>";

	reader.loadFromString(expectedXml);
	reader.load(runTime);

	EXPECT_THAT(runTime, RunTimeDTO(5));

	writer.save(runTime);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(RunTimeReaderWriterTest, runTimeId) {
	const string expectedXml = "<runtime id=\"1\"/>";

	reader.loadFromString(expectedXml);
	reader.load(runTime);

	EXPECT_THAT(runTime, RunTimeDTO().setId(1));

	writer.save(runTime);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(RunTimeReaderWriterTest, runTimeEmpty) {
	const string expectedXml = "<runtime/>";

	reader.loadFromString(expectedXml);
	reader.load(runTime);

	EXPECT_THAT(runTime, RunTimeDTO());

	writer.save(runTime);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(RunTimeReaderWriterTest, runTimeInvalid) {
	const string expectedXml = "<runtimeABC/>";

	reader.loadFromString(expectedXml);
	EXPECT_THROW(reader.load(runTime), RequiredTagMissing);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeReaderWriterTest, startTimeAll) {
	const string expectedXml = "<starttime id=\"1\">5</starttime>";

	reader.loadFromString(expectedXml);
	reader.load(startTime);

	EXPECT_THAT(startTime, StartTimeDTO(5).setId(1));

	writer.save(startTime);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(StartTimeReaderWriterTest, startTimeValue) {
	const string expectedXml = "<starttime>5</starttime>";

	reader.loadFromString(expectedXml);
	reader.load(startTime);

	EXPECT_THAT(startTime, StartTimeDTO(5));

	writer.save(startTime);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(StartTimeReaderWriterTest, startTimeId) {
	const string expectedXml = "<starttime id=\"1\"/>";

	reader.loadFromString(expectedXml);
	reader.load(startTime);

	EXPECT_THAT(startTime, StartTimeDTO().setId(1));

	writer.save(startTime);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(StartTimeReaderWriterTest, startTimeEmpty) {
	const string expectedXml = "<starttime/>";

	reader.loadFromString(expectedXml);
	reader.load(startTime);

	EXPECT_THAT(startTime, StartTimeDTO());

	writer.save(startTime);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(StartTimeReaderWriterTest, startTimeInvalid) {
	const string expectedXml = "<starttime12345/>";

	reader.loadFromString(expectedXml);
	EXPECT_THROW(reader.load(startTime), RequiredTagMissing);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerAll) {
	const string expectedXml =
			"<scheduler type=\"specified\">"
				"<day>true</day>"
				"<day>false</day>"
			"</scheduler>";

	reader.loadFromString(expectedXml);
	reader.load(scheduler);

	EXPECT_THAT(scheduler, SpecifiedSchedulerDTO(new list<bool>({ true, false })));

	writer.save(scheduler);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerEmpty) {
	const string expectedXml = "<scheduler type=\"specified\"/>";

	reader.loadFromString(expectedXml);
	reader.load(scheduler);

	EXPECT_THAT(scheduler, SpecifiedSchedulerDTO(new list<bool>()));

	writer.save(scheduler);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerInvalidType) {
	const string expectedXml = "<scheduler type=\"ABC\"/>";

	reader.loadFromString(expectedXml);
	EXPECT_THROW(reader.load(scheduler), BadSchedulerType);
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerInvalid) {
	const string expectedXml = "<ABCD/>";

	reader.loadFromString(expectedXml);
	EXPECT_THROW(reader.load(scheduler), RequiredTagMissing);
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
						"<day>true</day>"
						"<day>false</day>"
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

	reader.loadFromString(expectedXml);
	reader.load(program);

	EXPECT_THAT(program,
			ProgramDTO()
			.setName("abcdefg")
			.setSchedulerType("specified")
			.setSpecifiedScheduler(SpecifiedSchedulerDTO(new list<bool>({ true, false})))
			.setRunTimes(new list<RunTimeDTO>({
				RunTimeDTO(20).setId(15),
				RunTimeDTO(10).setId(25)}))
			.setStartTimes(new list<StartTimeDTO>({
				StartTimeDTO(50).setId(35),
				StartTimeDTO(30).setId(45),
				StartTimeDTO(100).setId(55)})
			));

	writer.save(program);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(ProgramReaderWriterTest, programName) {
	const string expectedXml =
			"<program>"
				"<name>abcdefg</name>"
			"</program>";

	reader.loadFromString(expectedXml);
	reader.load(program);

	EXPECT_THAT(program, ProgramDTO().setName("abcdefg"));

	writer.save(program);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(ProgramReaderWriterTest, programSchedulerType) {
	const string expectedXml =
			"<program>"
				"<schedulertype>specified</schedulertype>"
			"</program>";

	reader.loadFromString(expectedXml);
	reader.load(program);

	EXPECT_THAT(program, ProgramDTO().setSchedulerType("specified"));

	writer.save(program);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(ProgramReaderWriterTest, programSchedulers) {
	const string expectedXml =
			"<program>"
				"<schedulers>"
					"<scheduler type=\"specified\">"
						"<day>true</day>"
						"<day>false</day>"
					"</scheduler>"
				"</schedulers>"
			"</program>";

	reader.loadFromString(expectedXml);
	reader.load(program);

	EXPECT_THAT(program,
			ProgramDTO()
			.setSpecifiedScheduler(SpecifiedSchedulerDTO(new list<bool>({ true, false})))
			);

	writer.save(program);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(ProgramReaderWriterTest, programRunTimes) {
	const string expectedXml =
			"<program>"
				"<runtimes>"
					"<runtime id=\"15\">20</runtime>"
					"<runtime id=\"25\">10</runtime>"
				"</runtimes>"
			"</program>";

	reader.loadFromString(expectedXml);
	reader.load(program);

	EXPECT_THAT(program,
			ProgramDTO()
			.setRunTimes(new list<RunTimeDTO>({
				RunTimeDTO(20).setId(15),
				RunTimeDTO(10).setId(25)}))
			);

	writer.save(program);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
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

	reader.loadFromString(expectedXml);
	reader.load(program);

	EXPECT_THAT(program,
			ProgramDTO()
			.setStartTimes(new list<StartTimeDTO>({
				StartTimeDTO(50).setId(35),
				StartTimeDTO(30).setId(45),
				StartTimeDTO(100).setId(55)}))
			);

	writer.save(program);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(ProgramReaderWriterTest, programEmpty) {
	const string expectedXml = "<program/>";

	reader.loadFromString(expectedXml);
	reader.load(program);

	EXPECT_THAT(program, ProgramDTO());

	writer.save(program);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(ProgramReaderWriterTest, programInvalid) {
	const string expectedXml = "<prograsdcnnm/>";

	reader.loadFromString(expectedXml);
	EXPECT_THROW(reader.load(program), RequiredTagMissing);
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
								"<day>true</day>"
								"<day>false</day>"
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
								"<day>true</day>"
								"<day>true</day>"
								"<day>false</day>"
								"<day>false</day>"
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

	reader.loadFromString(expectedXml);
	reader.load(document);

	EXPECT_THAT(document, DocumentDTO(
		new list<ProgramDTO>({
			ProgramDTO()
				.setId(5)
				.setName("abcdefg")
				.setSchedulerType("specified")
				.setSpecifiedScheduler(SpecifiedSchedulerDTO(new list<bool>({ true, false })))
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
				.setSpecifiedScheduler(SpecifiedSchedulerDTO(new list<bool>({ true, true, false, false })))
				.setRunTimes(new list<RunTimeDTO>({
					RunTimeDTO(200).setId(115),
					RunTimeDTO(210).setId(125),
					RunTimeDTO(205).setId(135)}))
				.setStartTimes(new list<StartTimeDTO>({
					StartTimeDTO(130).setId(145),
					StartTimeDTO(150).setId(155)}))
			})));

	writer.save(document);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(DocumentReaderWriterTest, documentEmpty) {
	const string expectedXml = "<irrigation/>";

	reader.loadFromString(expectedXml);
	reader.load(document);

	EXPECT_THAT(document, DocumentDTO());

	writer.save(document);

	EXPECT_EQ(expectedXml, remove_xml_tag(writer.toString(false)));
}

TEST_F(DocumentReaderWriterTest, documentInvalid) {
	const string expectedXml = "<irrig/>";

	reader.loadFromString(expectedXml);
	EXPECT_THROW(reader.load(document), RequiredTagMissing);
}
