#include "XmlReaderWriterTest.h"
#include "Dto2Xml/ProgramSamples.h"
#include <list>
#include <string>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramWriterTest, save) {
	for (const auto& programSample : Dto2XmlTest::ProgramSampleList()) {
		const std::string actualXml = XmlWriter(false).save(programSample.getDto());
		const std::string expectedXml = programSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(remove_xml_tag(actualXml), Eq(expectedXml));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramReaderTest, load) {
	for (const auto& programSample : Dto2XmlTest::ProgramSampleList()) {
		const ProgramDTO actualDto = XmlReader().loadProgram(programSample.getXml());
		const ProgramDTO expectedDto = programSample.getDto();

		std::cout << actualDto << std::endl;

		EXPECT_THAT(actualDto, Eq(expectedDto));
	}
}

TEST(ProgramReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadProgram("<programABC/>"), RequiredTagMissing);
}

TEST(ProgramReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadProgram("jhvjhvjh"), XMLParseException);
}

TEST(ProgramReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadProgram("<abc/><123/>"), XMLParseException);
}
