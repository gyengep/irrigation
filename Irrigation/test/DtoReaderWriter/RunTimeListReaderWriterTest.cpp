#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/RunTimeListSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeListWriterTest, save) {
	for (const auto& runTimeSample : DtoReaderWriterTestSamples::RunTimeListSampleList()) {
		const std::string actualXml = XmlWriter(false).save(runTimeSample.getDtoList());
		const std::string expectedXml = runTimeSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlHeader(expectedXml)));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeListReaderTest, load) {
	for (const auto& runTimeSample : DtoReaderWriterTestSamples::RunTimeListSampleList()) {
		const std::list<RunTimeDTO> actualDtoList = XmlReader().loadRunTimeList(runTimeSample.getXml());
		const std::list<RunTimeDTO> expectedDtoList = runTimeSample.getDtoList();

		EXPECT_THAT(actualDtoList, Eq(expectedDtoList));
	}
}

TEST(RunTimeListReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadRunTimeList("<runtimeABC/>"), RequiredTagMissing);
}

TEST(RunTimeListReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadRunTimeList("jhvjhvjh"), XMLParseException);
}

TEST(RunTimeListReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadRunTimeList("<abc/><123/>"), XMLParseException);
}
