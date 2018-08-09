#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;


typedef std::pair<string, StartTimeDTO> TestDataType;

///////////////////////////////////////////////////////////////////////////////

const TestDataType testData_all(
		"<starttime id=\"8465\">8647</starttime>",
		StartTimeDTO(8647).setId(8465)
		);

const TestDataType testData_value(
		"<starttime>9437</starttime>",
		StartTimeDTO(9437)
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

TEST_F(StartTimeReaderTest, startTimeValue) {
	testStartTimeRead(testData_value, reader);
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

TEST_F(StartTimeWriterTest, startTimeValue) {
	testStartTimeWrite(testData_value, writer);
}

TEST_F(StartTimeWriterTest, startTimeId) {
	testStartTimeWrite(testData_id, writer);
}

TEST_F(StartTimeWriterTest, startTimeEmpty) {
	testStartTimeWrite(testData_empty, writer);
}
