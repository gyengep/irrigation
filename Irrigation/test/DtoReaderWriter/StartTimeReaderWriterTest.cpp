#include "XmlReaderWriterTest.h"
#include "Dto2Xml/StartTimeSamples.h"
#include <list>
#include <string>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeWriterTest, save) {
	for (const auto& startTimeSample : Dto2XmlTest::StartTimeSampleList()) {
		const std::string actualXml = XmlWriter(false).save(startTimeSample.getDto());
		const std::string expectedXml = startTimeSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(remove_xml_tag(actualXml), Eq(expectedXml));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeReaderTest, load) {
	for (const auto& startTimeSample : Dto2XmlTest::StartTimeSampleList()) {
		const StartTimeDTO actualDto = XmlReader().loadStartTime(startTimeSample.getXml());
		const StartTimeDTO expectedDto = startTimeSample.getDto();

		std::cout << actualDto << std::endl;
		
		EXPECT_THAT(actualDto, Eq(expectedDto));
	}
}

TEST(StartTimeReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadStartTime("<starttimeABC/>"), RequiredTagMissing);
}

TEST(StartTimeReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadStartTime("jhvjhvjh"), XMLParseException);
}

TEST(StartTimeReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadStartTime("<abc/><123/>"), XMLParseException);
}
