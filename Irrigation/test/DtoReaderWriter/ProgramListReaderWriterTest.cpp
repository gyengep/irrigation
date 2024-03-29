#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/ProgramListSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramListWriterTest, save) {
	for (const auto& programSample : DtoReaderWriterTestSamples::ProgramListSampleList()) {
		const std::string actualXml = XmlWriter(false).save(programSample.getDtoList());
		const std::string expectedXml = programSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlHeader(expectedXml)));
	}
}

TEST(ProgramListWriterTest, saveWithStyleSheet) {
	for (const auto& programSample : DtoReaderWriterTestSamples::ProgramListSampleList()) {
		const std::string actualXml = XmlWriter(false).save(programSample.getDtoList(), "stylesheet_123");
		const std::string expectedXml = programSample.getXmlWithoutContainers();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlAndStyleSheetHeader(expectedXml, "stylesheet_123")));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramListReaderTest, load) {
	for (const auto& programSample : DtoReaderWriterTestSamples::ProgramListSampleList()) {
		const ProgramDtoList actualDtoList = XmlReader().loadProgramDtoList(programSample.getXml());
		const ProgramDtoList expectedDtoList = programSample.getDtoList();

		EXPECT_THAT(actualDtoList, Eq(expectedDtoList));
	}
}

TEST(ProgramListReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadProgramDtoList("<programABC/>"), RequiredTagMissing);
}

TEST(ProgramListReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadProgramDtoList("jhvjhvjh"), XMLParseException);
}

TEST(ProgramListReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadProgramDtoList("<abc/><123/>"), XMLParseException);
}
