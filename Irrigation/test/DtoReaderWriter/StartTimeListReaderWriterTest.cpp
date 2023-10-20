#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/StartTimeListSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeListWriterTest, save) {
	for (const auto& startTimeSample : DtoReaderWriterTestSamples::StartTimeListSampleList()) {
		const std::string actualXml = XmlWriter(false).save(startTimeSample.getDtoList());
		const std::string expectedXml = startTimeSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlHeader(expectedXml)));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeListReaderTest, load) {
	for (const auto& startTimeSample : DtoReaderWriterTestSamples::StartTimeListSampleList()) {
		const StartTimeDtoList actualDtoList = XmlReader().loadStartTimeDtoList(startTimeSample.getXml());
		const StartTimeDtoList expectedDtoList = startTimeSample.getDtoList();

		EXPECT_THAT(actualDtoList, Eq(expectedDtoList));
	}
}

TEST(StartTimeListReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadStartTimeDtoList("<starttimeABC/>"), RequiredTagMissing);
}

TEST(StartTimeListReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadStartTimeDtoList("jhvjhvjh"), XMLParseException);
}

TEST(StartTimeListReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadStartTimeDtoList("<abc/><123/>"), XMLParseException);
}
