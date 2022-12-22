#include "XmlReaderWriterTest.h"
#include "Samples/SchedulersSamples.h"
#include <list>
#include <string>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(SchedulersWriterTest, save) {
	for (const auto& schedulersSample : DtoReaderWriterTestSamples::SchedulersSampleList()) {
		const std::string actualXml = XmlWriter(false).save(schedulersSample.getDto());
		const std::string expectedXml = schedulersSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(remove_xml_tag(actualXml), Eq(expectedXml));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(SchedulersReaderTest, load) {
	for (const auto& schedulersSample : DtoReaderWriterTestSamples::SchedulersSampleList()) {
		const SchedulersDTO actualDto = XmlReader().loadSchedulers(schedulersSample.getXml());
		const SchedulersDTO expectedDto = schedulersSample.getDto();

		std::cout << actualDto << std::endl;

		EXPECT_THAT(actualDto, Eq(expectedDto));
	}
}

TEST(SchedulersReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadSchedulers("<hotweatherABC/>"), RequiredTagMissing);
}

TEST(SchedulersReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadSchedulers("jhvjhvjh"), XMLParseException);
}

TEST(SchedulersReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadSchedulers("<abc/><123/>"), XMLParseException);
}
