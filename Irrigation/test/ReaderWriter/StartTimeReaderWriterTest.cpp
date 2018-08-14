#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;


typedef std::pair<string, StartTimeDTO> TestDataType;

///////////////////////////////////////////////////////////////////////////////

const TestDataType testData_all(
		"<starttime id=\"8465\"><hour>1</hour><minute>2</minute></starttime>",
		StartTimeDTO(1, 2).setId(8465)
		);

const TestDataType testData_hour(
		"<starttime><hour>11</hour></starttime>",
		StartTimeDTO().setHour(11)
		);

const TestDataType testData_minute(
		"<starttime><minute>12</minute></starttime>",
		StartTimeDTO().setMinute(12)
		);

const TestDataType testData_id(
		"<starttime id=\"45152\"/>",
		StartTimeDTO().setId(45152)
		);

const TestDataType testData_empty(
		"<starttime/>",
		StartTimeDTO()
		);

///////////////////////////////////////////////////////////////////////////////

void testStartTimeRead(const TestDataType& testData, XmlReader& reader) {
	const StartTimeDTO actualDto = reader.loadStartTime(testData.first);
	EXPECT_EQ(testData.second, actualDto);
}

void testStartTimeWrite(const TestDataType& testData, XmlWriter& writer) {
	const string actualXml = writer.save(testData.second, false);
	EXPECT_EQ(testData.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeReaderTest, startTimeInvalid) {
	EXPECT_THROW(reader.loadStartTime("<starttime12345/>"), RequiredTagMissing);
}

TEST_F(StartTimeReaderTest, startTimeNoneXml) {
	EXPECT_THROW(reader.loadStartTime("jhvjhvjh"), XMLParseException);
}

TEST_F(StartTimeReaderTest, startTimeInvalidXml) {
	EXPECT_THROW(reader.loadStartTime("<abc/><123/>"), XMLParseException);
}

TEST_F(StartTimeReaderTest, startTimeAll) {
	testStartTimeRead(testData_all, reader);
}

TEST_F(StartTimeReaderTest, startTimeHour) {
	testStartTimeRead(testData_hour, reader);
}

TEST_F(StartTimeReaderTest, startTimeMinute) {
	testStartTimeRead(testData_minute, reader);
}

TEST_F(StartTimeReaderTest, startTimeId) {
	testStartTimeRead(testData_id, reader);
}

TEST_F(StartTimeReaderTest, startTimeEmpty) {
	testStartTimeRead(testData_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeWriterTest, startTimeAll) {
	testStartTimeWrite(testData_all, writer);
}

TEST_F(StartTimeWriterTest, startTimeHour) {
	testStartTimeWrite(testData_hour, writer);
}

TEST_F(StartTimeWriterTest, startTimeMinute) {
	testStartTimeWrite(testData_minute, writer);
}

TEST_F(StartTimeWriterTest, startTimeId) {
	testStartTimeWrite(testData_id, writer);
}

TEST_F(StartTimeWriterTest, startTimeEmpty) {
	testStartTimeWrite(testData_empty, writer);
}
