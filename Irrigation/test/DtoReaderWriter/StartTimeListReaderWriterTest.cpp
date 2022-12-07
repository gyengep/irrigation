#include "XmlReaderWriterTest.h"
#include "Dto2Xml/StartTimeListSamples.h"
#include <list>
#include <string>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeListWriterTest, save) {
	for (const auto& startTimeSample : Dto2XmlTest::StartTimeListSampleList()) {
		const std::string actualXml = XmlWriter(false).save(startTimeSample.getDtoList());
		const std::string expectedXml = startTimeSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(remove_xml_tag(actualXml), Eq(expectedXml));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeListReaderTest, load) {
	for (const auto& startTimeSample : Dto2XmlTest::StartTimeListSampleList()) {
		const std::list<StartTimeDTO> actualDtoList = XmlReader().loadStartTimeList(startTimeSample.getXml());
		const std::list<StartTimeDTO> expectedDtoList = startTimeSample.getDtoList();

		EXPECT_THAT(actualDtoList, Eq(expectedDtoList));
	}
}

TEST(StartTimeListReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadStartTimeList("<starttimeABC/>"), RequiredTagMissing);
}

TEST(StartTimeListReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadStartTimeList("jhvjhvjh"), XMLParseException);
}

TEST(StartTimeListReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadStartTimeList("<abc/><123/>"), XMLParseException);
}
