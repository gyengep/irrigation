#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;


typedef std::pair<string, SpecifiedSchedulerDTO> TestDataType;

///////////////////////////////////////////////////////////////////////////////

const TestDataType testData_all(
		"<scheduler type=\"specified\">"
			"<adjustment>2390</adjustment>"
			"<days>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
				"<day>true</day>"
				"<day>true</day>"
			"</days>"
		"</scheduler>",
		SpecifiedSchedulerDTO(2390, new list<bool>({ true, false, false, true, true }))
		);

const TestDataType testData_adjustment(
		"<scheduler type=\"specified\">"
			"<adjustment>8560</adjustment>"
		"</scheduler>",
		SpecifiedSchedulerDTO().setAdjustment(8560)
		);

const TestDataType testData_values(
		"<scheduler type=\"specified\">"
			"<days>"
				"<day>true</day>"
				"<day>true</day>"
				"<day>true</day>"
				"<day>false</day>"
				"<day>false</day>"
			"</days>"
		"</scheduler>",
		SpecifiedSchedulerDTO().setValues(new list<bool>({ true, true, true, false, false }))
		);

const TestDataType testData_empty(
		"<scheduler type=\"specified\"/>",
		SpecifiedSchedulerDTO()
		);


///////////////////////////////////////////////////////////////////////////////

void testSpecifiedSchedulerRead(const TestDataType& testData, XmlReader& reader) {
	const SpecifiedSchedulerDTO actualDto = reader.loadSpecifiedScheduler(testData.first);
	EXPECT_EQ(testData.second, actualDto);
}

void testSpecifiedSchedulerWrite(const TestDataType& testData, XmlWriter& writer) {
	const string actualXml = writer.save(testData.second, false);
	EXPECT_EQ(testData.first, remove_xml_tag(actualXml));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(SpecifiedSchedulerReaderTest, schedulerInvalidType) {
	EXPECT_THROW(reader.loadSpecifiedScheduler("<scheduler type=\"ABC\"/>"), invalid_argument);
}

TEST_F(SpecifiedSchedulerReaderTest, schedulerInvalid) {
	EXPECT_THROW(reader.loadSpecifiedScheduler("<ABCD/>"), RequiredTagMissing);
}

TEST_F(SpecifiedSchedulerReaderTest, schedulerNoneXml) {
	EXPECT_THROW(reader.loadSpecifiedScheduler("jhvjhvjh"), XMLParseException);
}

TEST_F(SpecifiedSchedulerReaderTest, schedulerInvalidXml) {
	EXPECT_THROW(reader.loadSpecifiedScheduler("<abc/><123/>"), XMLParseException);
}

TEST_F(SpecifiedSchedulerReaderTest, schedulerAll) {
	testSpecifiedSchedulerRead(testData_all, reader);
}

TEST_F(SpecifiedSchedulerReaderTest, schedulerAdjustment) {
	testSpecifiedSchedulerRead(testData_adjustment, reader);
}

TEST_F(SpecifiedSchedulerReaderTest, schedulerValues) {
	testSpecifiedSchedulerRead(testData_values, reader);
}

TEST_F(SpecifiedSchedulerReaderTest, schedulerEmpty) {
	testSpecifiedSchedulerRead(testData_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(SpecifiedSchedulerWriterTest, schedulerAll) {
	testSpecifiedSchedulerWrite(testData_all, writer);
}

TEST_F(SpecifiedSchedulerWriterTest, schedulerAdjustment) {
	testSpecifiedSchedulerWrite(testData_adjustment, writer);
}

TEST_F(SpecifiedSchedulerWriterTest, schedulerValues) {
	testSpecifiedSchedulerWrite(testData_values, writer);
}

TEST_F(SpecifiedSchedulerWriterTest, schedulerEmpty) {
	testSpecifiedSchedulerWrite(testData_empty, writer);
}
