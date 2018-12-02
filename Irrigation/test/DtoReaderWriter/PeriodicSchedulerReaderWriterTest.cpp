#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;


typedef pair<string, PeriodicSchedulerDTO> TestDataType;

///////////////////////////////////////////////////////////////////////////////

const TestDataType testData_all(
		"<scheduler type=\"periodic\">"
			"<adjustment>239</adjustment>"
			"<days>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
				"<day>true</day>"
				"<day>true</day>"
			"</days>"
			"<periodStartDate>"
				"<year>2018</year>"
				"<month>11</month>"
				"<day>16</day>"
			"</periodStartDate>"
		"</scheduler>",
		PeriodicSchedulerDTO(239, list<bool>({ true, false, false, true, true }), 2018, 11, 16)
		);

const TestDataType testData_adjustment(
		"<scheduler type=\"periodic\">"
			"<adjustment>8560</adjustment>"
		"</scheduler>",
		PeriodicSchedulerDTO().setAdjustment(8560)
		);

const TestDataType testData_values(
		"<scheduler type=\"periodic\">"
			"<days>"
				"<day>true</day>"
				"<day>true</day>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
			"</days>"
		"</scheduler>",
		PeriodicSchedulerDTO().setValues(list<bool>({ true, true, true, false, false }))
		);

const TestDataType testData_date(
		"<scheduler type=\"periodic\">"
			"<periodStartDate>"
				"<year>2018</year>"
				"<month>11</month>"
				"<day>16</day>"
			"</periodStartDate>"
		"</scheduler>",
		PeriodicSchedulerDTO().setPeriodStartYear(2018).setPeriodStartMonth(11).setPeriodStartDay(16)
		);

const TestDataType testData_empty(
		"<scheduler type=\"periodic\"/>",
		PeriodicSchedulerDTO()
		);


///////////////////////////////////////////////////////////////////////////////

void testPeriodicSchedulerRead(const TestDataType& testData, XmlReader& reader) {
	const PeriodicSchedulerDTO actualDto = reader.loadPeriodicScheduler(testData.first);
	EXPECT_EQ(testData.second, actualDto);
}

void testPeriodicSchedulerWrite(const TestDataType& testData, XmlWriter& writer) {
	const string actualXml = writer.save(testData.second);
	EXPECT_EQ(testData.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(PeriodicSchedulerReaderTest, schedulerInvalidType) {
	EXPECT_THROW(reader.loadPeriodicScheduler("<scheduler type=\"ABC\"/>"), invalid_argument);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerInvalid) {
	EXPECT_THROW(reader.loadPeriodicScheduler("<ABCD/>"), RequiredTagMissing);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerNoneXml) {
	EXPECT_THROW(reader.loadPeriodicScheduler("jhvjhvjh"), XMLParseException);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerInvalidXml) {
	EXPECT_THROW(reader.loadPeriodicScheduler("<abc/><123/>"), XMLParseException);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerAll) {
	testPeriodicSchedulerRead(testData_all, reader);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerAdjustment) {
	testPeriodicSchedulerRead(testData_adjustment, reader);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerValues) {
	testPeriodicSchedulerRead(testData_values, reader);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerDate) {
	testPeriodicSchedulerRead(testData_date, reader);
}

TEST_F(PeriodicSchedulerReaderTest, schedulerEmpty) {
	testPeriodicSchedulerRead(testData_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(PeriodicSchedulerWriterTest, schedulerAll) {
	testPeriodicSchedulerWrite(testData_all, writer);
}

TEST_F(PeriodicSchedulerWriterTest, schedulerAdjustment) {
	testPeriodicSchedulerWrite(testData_adjustment, writer);
}

TEST_F(PeriodicSchedulerWriterTest, schedulerValues) {
	testPeriodicSchedulerWrite(testData_values, writer);
}

TEST_F(PeriodicSchedulerWriterTest, schedulerDate) {
	testPeriodicSchedulerWrite(testData_date, writer);
}

TEST_F(PeriodicSchedulerWriterTest, schedulerEmpty) {
	testPeriodicSchedulerWrite(testData_empty, writer);
}
