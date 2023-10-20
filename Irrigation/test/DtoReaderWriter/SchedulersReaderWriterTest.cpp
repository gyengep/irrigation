#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/SchedulersSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(SchedulersWriterTest, save) {
	for (const auto& schedulersSample : DtoReaderWriterTestSamples::SchedulersSampleList()) {
		const std::string actualXml = XmlWriter(false).save(schedulersSample.getDto());
		const std::string expectedXml = schedulersSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlHeader(expectedXml)));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(SchedulersReaderTest, load) {
	for (const auto& schedulersSample : DtoReaderWriterTestSamples::SchedulersSampleList()) {
		const SchedulersDto actualDto = XmlReader().loadSchedulersDto(schedulersSample.getXml());
		const SchedulersDto expectedDto = schedulersSample.getDto();

		std::cout << actualDto << std::endl;

		EXPECT_THAT(actualDto, Eq(expectedDto));
	}
}

TEST(SchedulersReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadSchedulersDto("<hotweatherABC/>"), RequiredTagMissing);
}

TEST(SchedulersReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadSchedulersDto("jhvjhvjh"), XMLParseException);
}

TEST(SchedulersReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadSchedulersDto("<abc/><123/>"), XMLParseException);
}
