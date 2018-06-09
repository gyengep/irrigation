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
/*
TEST(XmlReader, loadNoneXml) {
	XmlReader reader;
	EXPECT_THROW(reader.loadFromString("jhvjhvjh"), XmlParsingError);
}


TEST(XmlReader, loadInvalidXml) {
	XmlReader reader;
	EXPECT_THROW(reader.loadFromString("<abc/><123/>"), XmlParsingError);
}
*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeReaderWriterTest, runTimeAll) {
	const string expectedXml = "<runtime id=\"1\">5</runtime>";

	reader.load(runTime, expectedXml);

	EXPECT_THAT(runTime, RunTimeDTO(5).setId(1));

	const std::string actualXml = writer.save(runTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(RunTimeReaderWriterTest, runTimeValue) {
	const string expectedXml = "<runtime>5</runtime>";

	reader.load(runTime, expectedXml);

	EXPECT_THAT(runTime, RunTimeDTO(5));

	const std::string actualXml = writer.save(runTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(RunTimeReaderWriterTest, runTimeId) {
	const string expectedXml = "<runtime id=\"1\"/>";

	reader.load(runTime, expectedXml);

	EXPECT_THAT(runTime, RunTimeDTO().setId(1));

	const std::string actualXml = writer.save(runTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(RunTimeReaderWriterTest, runTimeEmpty) {
	const string expectedXml = "<runtime/>";

	reader.load(runTime, expectedXml);

	EXPECT_THAT(runTime, RunTimeDTO());

	const std::string actualXml = writer.save(runTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(RunTimeReaderWriterTest, runTimeInvalid) {
	const string expectedXml = "<runtimeABC/>";

	EXPECT_THROW(reader.load(runTime, expectedXml), RequiredTagMissing);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeReaderWriterTest, startTimeAll) {
	const string expectedXml = "<starttime id=\"1\">5</starttime>";

	reader.load(startTime, expectedXml);

	EXPECT_THAT(startTime, StartTimeDTO(5).setId(1));

	const std::string actualXml = writer.save(startTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(StartTimeReaderWriterTest, startTimeValue) {
	const string expectedXml = "<starttime>5</starttime>";

	reader.load(startTime, expectedXml);

	EXPECT_THAT(startTime, StartTimeDTO(5));

	const std::string actualXml = writer.save(startTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(StartTimeReaderWriterTest, startTimeId) {
	const string expectedXml = "<starttime id=\"1\"/>";

	reader.load(startTime, expectedXml);

	EXPECT_THAT(startTime, StartTimeDTO().setId(1));

	const std::string actualXml = writer.save(startTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(StartTimeReaderWriterTest, startTimeEmpty) {
	const string expectedXml = "<starttime/>";

	reader.load(startTime, expectedXml);

	EXPECT_THAT(startTime, StartTimeDTO());

	const std::string actualXml = writer.save(startTime, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(StartTimeReaderWriterTest, startTimeInvalid) {
	const string expectedXml = "<starttime12345/>";

	EXPECT_THROW(reader.load(startTime, expectedXml), RequiredTagMissing);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerAll) {
	const string expectedXml =
			"<scheduler type=\"specified\">"
				"<day>true</day>"
				"<day>false</day>"
			"</scheduler>";

	reader.load(scheduler, expectedXml);

	EXPECT_THAT(scheduler, SpecifiedSchedulerDTO(new list<bool>({ true, false })));

	const std::string actualXml = writer.save(scheduler, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerEmpty) {
	const string expectedXml = "<scheduler type=\"specified\"/>";

	reader.load(scheduler, expectedXml);

	EXPECT_THAT(scheduler, SpecifiedSchedulerDTO(new list<bool>()));

	const std::string actualXml = writer.save(scheduler, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerInvalidType) {
	const string expectedXml = "<scheduler type=\"ABC\"/>";

	EXPECT_THROW(reader.load(scheduler, expectedXml), BadSchedulerType);
}

TEST_F(SpecifiedSchedulerReaderWriterTest, schedulerInvalid) {
	const string expectedXml = "<ABCD/>";

	EXPECT_THROW(reader.load(scheduler, expectedXml), RequiredTagMissing);
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

	reader.load(program, expectedXml);

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

	const std::string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(ProgramReaderWriterTest, programName) {
	const string expectedXml =
			"<program>"
				"<name>abcdefg</name>"
			"</program>";

	reader.load(program, expectedXml);

	EXPECT_THAT(program, ProgramDTO().setName("abcdefg"));

	const std::string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(ProgramReaderWriterTest, programSchedulerType) {
	const string expectedXml =
			"<program>"
				"<schedulertype>specified</schedulertype>"
			"</program>";

	reader.load(program, expectedXml);

	EXPECT_THAT(program, ProgramDTO().setSchedulerType("specified"));

	const std::string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
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

	reader.load(program, expectedXml);

	EXPECT_THAT(program,
			ProgramDTO()
			.setSpecifiedScheduler(SpecifiedSchedulerDTO(new list<bool>({ true, false})))
			);

	const std::string actualXml = writer.save(program, false);
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

	reader.load(program, expectedXml);

	EXPECT_THAT(program,
			ProgramDTO()
			.setRunTimes(new list<RunTimeDTO>({
				RunTimeDTO(20).setId(15),
				RunTimeDTO(10).setId(25)}))
			);

	const std::string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
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

	reader.load(program, expectedXml);

	EXPECT_THAT(program,
			ProgramDTO()
			.setStartTimes(new list<StartTimeDTO>({
				StartTimeDTO(50).setId(35),
				StartTimeDTO(30).setId(45),
				StartTimeDTO(100).setId(55)}))
			);

	const std::string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(ProgramReaderWriterTest, programEmpty) {
	const string expectedXml = "<program/>";

	reader.load(program, expectedXml);

	EXPECT_THAT(program, ProgramDTO());

	const std::string actualXml = writer.save(program, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(ProgramReaderWriterTest, programInvalid) {
	const string expectedXml = "<prograsdcnnm/>";

	EXPECT_THROW(reader.load(program, expectedXml), RequiredTagMissing);
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

	reader.load(document, expectedXml);

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

	const std::string actualXml = writer.save(document, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(DocumentReaderWriterTest, documentEmpty) {
	const string expectedXml = "<irrigation/>";

	reader.load(document, expectedXml);

	EXPECT_THAT(document, DocumentDTO());

	const std::string actualXml = writer.save(document, false);
	EXPECT_EQ(expectedXml, remove_xml_tag(actualXml));
}

TEST_F(DocumentReaderWriterTest, documentInvalid) {
	const string expectedXml = "<irrig/>";

	EXPECT_THROW(reader.load(document, expectedXml), RequiredTagMissing);
}
