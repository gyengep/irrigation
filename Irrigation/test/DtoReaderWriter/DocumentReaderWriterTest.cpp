#include "XmlReaderWriterTest.h"
#include "Samples/DocumentSamples.h"
#include <list>
#include <string>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(DocumentWriterTest, save) {
	for (const auto& documentSample : DtoReaderWriterTestSamples::DocumentSampleList()) {
		const std::string actualXml = XmlWriter(false).save(documentSample.getDto());
		const std::string expectedXml = documentSample.getXml();

		std::cout << actualXml << std::endl;

		EXPECT_THAT(remove_xml_tag(actualXml), Eq(expectedXml));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(DocumentReaderTest, load) {
	for (const auto& documentSample : DtoReaderWriterTestSamples::DocumentSampleList()) {
		const DocumentDTO actualDto = XmlReader().loadDocument(documentSample.getXml());
		const DocumentDTO expectedDto = documentSample.getDto();

		std::cout << actualDto << std::endl;

		EXPECT_THAT(actualDto, Eq(expectedDto));
	}
}

TEST(DocumentReaderTest, loadInvalid) {
	EXPECT_THROW(XmlReader().loadDocument("<irrigABC/>"), RequiredTagMissing);
}

TEST(DocumentReaderTest, loadNoneXml) {
	EXPECT_THROW(XmlReader().loadDocument("jhvjhvjh"), XMLParseException);
}

TEST(DocumentReaderTest, loadInvalidXml) {
	EXPECT_THROW(XmlReader().loadDocument("<abc/><123/>"), XMLParseException);
}
