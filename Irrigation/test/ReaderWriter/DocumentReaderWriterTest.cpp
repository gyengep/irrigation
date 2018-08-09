#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;


typedef std::pair<string, DocumentDTO> TestDataType;

///////////////////////////////////////////////////////////////////////////////

const TestDataType testData_empty(
		"<irrigation/>",
		DocumentDTO()
		);

const TestDataType testData_programs(
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
		"</irrigation>",
		DocumentDTO(new list<ProgramDTO>({
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
			}))
		);

const TestDataType testData_programsWithoutId(
		"<irrigation>"
			"<programs>"
				"<program>"
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
						"<runtime>20</runtime>"
						"<runtime>10</runtime>"
					"</runtimes>"
					"<starttimes>"
						"<starttime>50</starttime>"
						"<starttime>30</starttime>"
						"<starttime>100</starttime>"
					"</starttimes>"
				"</program>"
			"</programs>"
		"</irrigation>",
		DocumentDTO(new list<ProgramDTO>({
			ProgramDTO()
				.setName("abcdefg")
				.setSchedulerType("specified")
				.setSpecifiedScheduler(SpecifiedSchedulerDTO(130, new list<bool>({ true, false })))
				.setRunTimes(new list<RunTimeDTO>({
					RunTimeDTO(20),
					RunTimeDTO(10)}))
				.setStartTimes(new list<StartTimeDTO>({
					StartTimeDTO(50),
					StartTimeDTO(30),
					StartTimeDTO(100)}))

			}))
		);


///////////////////////////////////////////////////////////////////////////////

void testDocumentRead(const TestDataType& testData, XmlReader& reader) {
	const DocumentDTO actualDto = reader.loadDocument(testData.first);
	EXPECT_EQ(testData.second, actualDto);
}

void testDocumentWrite(const TestDataType& testData, XmlWriter& writer) {
	const string actualXml = writer.save(testData.second, false);
	EXPECT_EQ(testData.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(DocumentReaderTest, documentInvalid) {
	EXPECT_THROW(reader.loadDocument("<irrig/>"), RequiredTagMissing);
}

TEST_F(DocumentReaderTest, documentNoneXml) {
	EXPECT_THROW(reader.loadDocument("jhvjhvjh"), XMLParseException);
}

TEST_F(DocumentReaderTest, documentInvalidXml) {
	EXPECT_THROW(reader.loadDocument("<abc/><123/>"), XMLParseException);
}

TEST_F(DocumentReaderTest, documentProgramsWithoutId) {
	testDocumentRead(testData_programsWithoutId, reader);
}

TEST_F(DocumentReaderTest, documentPrograms) {
	testDocumentRead(testData_programs, reader);
}

TEST_F(DocumentReaderTest, documentEmpty) {
	testDocumentRead(testData_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(DocumentWriterTest, documentPrograms) {
	testDocumentWrite(testData_programs, writer);
}

TEST_F(DocumentWriterTest, documentEmpty) {
	testDocumentWrite(testData_empty, writer);
}
