#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"
#include "Samples/LogEntryListSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(LogEntryListWriterTest, save) {
	for (const auto& logEntryListSample : DtoReaderWriterTestSamples::LogEntryListSampleList()) {
		const std::string actualXml = XmlWriter(false).save(logEntryListSample.getDto(), "MyStyleSheetFile");
		const std::string expectedXml = logEntryListSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(actualXml, Eq(prependXmlAndStyleSheetHeader(expectedXml, "MyStyleSheetFile")));
	}
}
