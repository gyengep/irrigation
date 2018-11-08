#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;


typedef pair<string, DocumentDTO> TestDataType;

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
						"<runtime id=\"15\"><minute>25</minute><second>54</second></runtime>"
						"<runtime id=\"25\"><minute>35</minute><second>44</second></runtime>"
					"</runtimes>"
					"<starttimes>"
						"<starttime id=\"35\"><hour>15</hour><minute>51</minute></starttime>"
						"<starttime id=\"45\"><hour>16</hour><minute>52</minute></starttime>"
						"<starttime id=\"55\"><hour>17</hour><minute>53</minute></starttime>"
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
						"<runtime id=\"115\"><minute>30</minute><second>15</second></runtime>"
						"<runtime id=\"125\"><minute>40</minute><second>25</second></runtime>"
						"<runtime id=\"135\"><minute>50</minute><second>35</second></runtime>"
					"</runtimes>"
					"<starttimes>"
						"<starttime id=\"145\"><hour>20</hour><minute>30</minute></starttime>"
						"<starttime id=\"155\"><hour>15</hour><minute>25</minute></starttime>"
					"</starttimes>"
				"</program>"
			"</programs>"
		"</irrigation>",
		DocumentDTO(list<ProgramDTO>({
			ProgramDTO()
				.setId(5)
				.setName("abcdefg")
				.setSchedulerType("specified")
				.setSpecifiedScheduler(SpecifiedSchedulerDTO(130, list<bool>({ true, false })))
				.setRunTimes(list<RunTimeDTO>({
					RunTimeDTO(25, 54).setId(15),
					RunTimeDTO(35, 44).setId(25)}))
				.setStartTimes(list<StartTimeDTO>({
					StartTimeDTO(15, 51).setId(35),
					StartTimeDTO(16, 52).setId(45),
					StartTimeDTO(17, 53).setId(55)})),
			ProgramDTO()
				.setId(6)
				.setName("123456")
				.setSchedulerType("specified")
				.setSpecifiedScheduler(SpecifiedSchedulerDTO(140, list<bool>({ true, true, false, false })))
				.setRunTimes(list<RunTimeDTO>({
					RunTimeDTO(30, 15).setId(115),
					RunTimeDTO(40, 25).setId(125),
					RunTimeDTO(50, 35).setId(135)}))
				.setStartTimes(list<StartTimeDTO>({
					StartTimeDTO(20, 30).setId(145),
					StartTimeDTO(15, 25).setId(155)}))
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
						"<runtime><minute>25</minute><second>54</second></runtime>"
						"<runtime><minute>52</minute><second>45</second></runtime>"
					"</runtimes>"
					"<starttimes>"
						"<starttime><hour>20</hour><minute>30</minute></starttime>"
						"<starttime><hour>19</hour><minute>32</minute></starttime>"
						"<starttime><hour>18</hour><minute>36</minute></starttime>"
					"</starttimes>"
				"</program>"
			"</programs>"
		"</irrigation>",
		DocumentDTO(list<ProgramDTO>({
			ProgramDTO()
				.setName("abcdefg")
				.setSchedulerType("specified")
				.setSpecifiedScheduler(SpecifiedSchedulerDTO(130, list<bool>({ true, false })))
				.setRunTimes(list<RunTimeDTO>({
					RunTimeDTO(25, 54),
					RunTimeDTO(52, 45)}))
				.setStartTimes(list<StartTimeDTO>({
					StartTimeDTO(20, 30),
					StartTimeDTO(19, 32),
					StartTimeDTO(18, 36)}))

			}))
		);


///////////////////////////////////////////////////////////////////////////////

void testDocumentRead(const TestDataType& testData, XmlReader& reader) {
	const DocumentDTO actualDto = reader.loadDocument(testData.first);
	EXPECT_EQ(testData.second, actualDto);
}

void testDocumentWrite(const TestDataType& testData, XmlWriter& writer) {
	const string actualXml = writer.save(testData.second);
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
