#include "XmlReaderWriterTest.h"
#include "Dto2Xml/ProgramListSamples.h"
#include <list>
#include <string>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramListWriterTest, save) {
	for (const auto& programSample : Dto2XmlTest::ProgramListSampleList()) {
		const std::string actualXml = XmlWriter(false).save(programSample.getDtoList());
		const std::string expectedXml = programSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(remove_xml_tag(actualXml), Eq(expectedXml));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramListReaderTest, load) {
	for (const auto& programSample : Dto2XmlTest::ProgramListSampleList()) {
		const std::list<ProgramDTO> actualDtoList = XmlReader().loadProgramList(programSample.getXml());
		const std::list<ProgramDTO> expectedDtoList = programSample.getDtoList();

		EXPECT_THAT(actualDtoList, Eq(expectedDtoList));
	}
}

TEST(ProgramListReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadProgramList("<programABC/>"), RequiredTagMissing);
}

TEST(ProgramListReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadProgramList("jhvjhvjh"), XMLParseException);
}

TEST(ProgramListReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadProgramList("<abc/><123/>"), XMLParseException);
}
