#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/RunTimeSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeWriterTest, save) {
	for (const auto& runTimeSample : DtoReaderWriterTestSamples::RunTimeSampleList()) {
		const std::string actualXml = XmlWriter(false).save(runTimeSample.getDto());
		const std::string expectedXml = runTimeSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlHeader(expectedXml)));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeReaderTest, load) {
	for (const auto& runTimeSample : DtoReaderWriterTestSamples::RunTimeSampleList()) {
		const RunTimeDTO actualDto = XmlReader().loadRunTime(runTimeSample.getXml());
		const RunTimeDTO expectedDto = runTimeSample.getDto();

		std::cout << actualDto << std::endl;

		EXPECT_THAT(actualDto, Eq(expectedDto));
	}
}

TEST(RunTimeReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadRunTime("<runtimeABC/>"), RequiredTagMissing);
}

TEST(RunTimeReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadRunTime("jhvjhvjh"), XMLParseException);
}

TEST(RunTimeReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadRunTime("<abc/><123/>"), XMLParseException);
}
