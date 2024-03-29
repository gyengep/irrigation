#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/ProgramSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramWriterTest, save) {
	for (const auto& programSample : DtoReaderWriterTestSamples::ProgramSampleList()) {
		const std::string actualXml = XmlWriter(false).save(programSample.getDto());
		const std::string expectedXml = programSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlHeader(expectedXml)));
	}
}

TEST(ProgramWriterTest, saveWithStyleSheet) {
	for (const auto& programSample : DtoReaderWriterTestSamples::ProgramSampleList()) {
		const std::string actualXml = XmlWriter(false).save(programSample.getDto(), "MyStyleSheet");
		const std::string expectedXml = programSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlAndStyleSheetHeader(expectedXml, "MyStyleSheet")));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramReaderTest, load) {
	for (const auto& programSample : DtoReaderWriterTestSamples::ProgramSampleList()) {
		const ProgramDto actualDto = XmlReader().loadProgramDto(programSample.getXml());
		const ProgramDto expectedDto = programSample.getDto();

		std::cout << actualDto << std::endl;

		EXPECT_THAT(actualDto, Eq(expectedDto));
	}
}

TEST(ProgramReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadProgramDto("<programABC/>"), RequiredTagMissing);
}

TEST(ProgramReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadProgramDto("jhvjhvjh"), XMLParseException);
}

TEST(ProgramReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadProgramDto("<abc/><123/>"), XMLParseException);
}
