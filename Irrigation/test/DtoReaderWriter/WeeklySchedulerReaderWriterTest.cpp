#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;


typedef pair<string, WeeklySchedulerDTO> TestDataType;

///////////////////////////////////////////////////////////////////////////////

const TestDataType testData_all(
		"<scheduler type=\"weekly\">"
			"<adjustment>2390</adjustment>"
			"<days>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
				"<day>true</day>"
				"<day>true</day>"
			"</days>"
		"</scheduler>",
		WeeklySchedulerDTO(2390, list<bool>({ true, false, false, true, true }))
		);

const TestDataType testData_adjustment(
		"<scheduler type=\"weekly\">"
			"<adjustment>8560</adjustment>"
		"</scheduler>",
		WeeklySchedulerDTO().setAdjustment(8560)
		);

const TestDataType testData_values(
		"<scheduler type=\"weekly\">"
			"<days>"
				"<day>true</day>"
				"<day>true</day>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
			"</days>"
		"</scheduler>",
		WeeklySchedulerDTO().setValues(list<bool>({ true, true, true, false, false }))
		);

const TestDataType testData_empty(
		"<scheduler type=\"weekly\"/>",
		WeeklySchedulerDTO()
		);


///////////////////////////////////////////////////////////////////////////////

void testWeeklySchedulerRead(const TestDataType& testData, XmlReader& reader) {
	const WeeklySchedulerDTO actualDto = reader.loadWeeklyScheduler(testData.first);
	EXPECT_EQ(testData.second, actualDto);
}

void testWeeklySchedulerWrite(const TestDataType& testData, XmlWriter& writer) {
	const string actualXml = writer.save(testData.second);
	EXPECT_EQ(testData.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WeeklySchedulerReaderTest, schedulerInvalidType) {
	EXPECT_THROW(reader.loadWeeklyScheduler("<scheduler type=\"ABC\"/>"), invalid_argument);
}

TEST_F(WeeklySchedulerReaderTest, schedulerInvalid) {
	EXPECT_THROW(reader.loadWeeklyScheduler("<ABCD/>"), RequiredTagMissing);
}

TEST_F(WeeklySchedulerReaderTest, schedulerNoneXml) {
	EXPECT_THROW(reader.loadWeeklyScheduler("jhvjhvjh"), XMLParseException);
}

TEST_F(WeeklySchedulerReaderTest, schedulerInvalidXml) {
	EXPECT_THROW(reader.loadWeeklyScheduler("<abc/><123/>"), XMLParseException);
}

TEST_F(WeeklySchedulerReaderTest, schedulerAll) {
	testWeeklySchedulerRead(testData_all, reader);
}

TEST_F(WeeklySchedulerReaderTest, schedulerAdjustment) {
	testWeeklySchedulerRead(testData_adjustment, reader);
}

TEST_F(WeeklySchedulerReaderTest, schedulerValues) {
	testWeeklySchedulerRead(testData_values, reader);
}

TEST_F(WeeklySchedulerReaderTest, schedulerEmpty) {
	testWeeklySchedulerRead(testData_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WeeklySchedulerWriterTest, schedulerAll) {
	testWeeklySchedulerWrite(testData_all, writer);
}

TEST_F(WeeklySchedulerWriterTest, schedulerAdjustment) {
	testWeeklySchedulerWrite(testData_adjustment, writer);
}

TEST_F(WeeklySchedulerWriterTest, schedulerValues) {
	testWeeklySchedulerWrite(testData_values, writer);
}

TEST_F(WeeklySchedulerWriterTest, schedulerEmpty) {
	testWeeklySchedulerWrite(testData_empty, writer);
}
