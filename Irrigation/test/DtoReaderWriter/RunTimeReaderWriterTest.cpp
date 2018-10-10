#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;


typedef std::pair<string, RunTimeDTO> TestDataType;

///////////////////////////////////////////////////////////////////////////////

const TestDataType testData_all(
		"<runtime id=\"547\"><minute>52</minute><second>43</second></runtime>",
		RunTimeDTO(52, 43).setId(547)
		);

const TestDataType testData_minute(
		"<runtime><minute>17</minute></runtime>",
		RunTimeDTO().setMinutes(17)
		);

const TestDataType testData_second(
		"<runtime><second>34</second></runtime>",
		RunTimeDTO().setSeconds(34)
		);

const TestDataType testData_id(
		"<runtime id=\"9348\"/>",
		RunTimeDTO().setId(9348)
		);

const TestDataType testData_empty(
		"<runtime/>",
		RunTimeDTO()
		);

///////////////////////////////////////////////////////////////////////////////

void testRunTimeRead(const TestDataType& testData, XmlReader& reader) {
	const RunTimeDTO actualDto = reader.loadRunTime(testData.first);
	EXPECT_EQ(testData.second, actualDto);
}

void testRunTimeWrite(const TestDataType& testData, XmlWriter& writer) {
	const string actualXml = writer.save(testData.second);
	EXPECT_EQ(testData.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeReaderTest, runTimeInvalid) {
	EXPECT_THROW(reader.loadRunTime("<runtimeABC/>"), RequiredTagMissing);
}

TEST_F(RunTimeReaderTest, runTimeNoneXml) {
	EXPECT_THROW(reader.loadRunTime("jhvjhvjh"), XMLParseException);
}

TEST_F(RunTimeReaderTest, runTimeInvalidXml) {
	EXPECT_THROW(reader.loadRunTime("<abc/><123/>"), XMLParseException);
}

TEST_F(RunTimeReaderTest, runTimeAll) {
	testRunTimeRead(testData_all, reader);
}

TEST_F(RunTimeReaderTest, runTimeMinute) {
	testRunTimeRead(testData_minute, reader);
}

TEST_F(RunTimeReaderTest, runTimeSecond) {
	testRunTimeRead(testData_second, reader);
}

TEST_F(RunTimeReaderTest, runTimeId) {
	testRunTimeRead(testData_id, reader);
}

TEST_F(RunTimeReaderTest, runTimeEmpty) {
	testRunTimeRead(testData_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeWriterTest, runTimeAll) {
	testRunTimeWrite(testData_all, writer);
}

TEST_F(RunTimeWriterTest, runTimeMinute) {
	testRunTimeWrite(testData_minute, writer);
}

TEST_F(RunTimeWriterTest, runTimeSecond) {
	testRunTimeWrite(testData_second, writer);
}

TEST_F(RunTimeWriterTest, runTimeId) {
	testRunTimeWrite(testData_id, writer);
}

TEST_F(RunTimeWriterTest, runTimeEmpty) {
	testRunTimeWrite(testData_empty, writer);
}
