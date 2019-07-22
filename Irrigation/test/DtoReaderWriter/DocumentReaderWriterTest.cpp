#include <list>
#include <string>
#include "XmlReaderWriterTest.h"
#include "DocumentSamples.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void testDocumentRead(const DocumentSample& documentSample, XmlReader& reader) {
	const DocumentDTO actualDto = reader.loadDocument(documentSample.first);
	EXPECT_THAT(actualDto, Eq(documentSample.second));
}


void testDocumentWrite(const DocumentSample& documentSample, XmlWriter& writer) {
	const string actualXml = writer.save(documentSample.second);
	EXPECT_THAT(remove_xml_tag(actualXml), Eq(documentSample.first));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(DocumentReaderTest, documentInvalid) {
	EXPECT_THROW(reader.loadDocument("<irrig/>"), RequiredTagMissing);
}

TEST_F(DocumentReaderTest, documentNoneXml) {
	EXPECT_THROW(reader.loadDocument("jhvjhvjh"), XMLParseException);
}

TEST_F(DocumentReaderTest, documentInvalidXml) {
	EXPECT_THROW(reader.loadDocument("<abc/><123/>"), XMLParseException);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(DocumentReaderTest, document1) {
	testDocumentRead(documentSample_1, reader);
}

TEST_F(DocumentReaderTest, document2) {
	testDocumentRead(documentSample_2, reader);
}

TEST_F(DocumentReaderTest, documentEmpty) {
	testDocumentRead(documentSample_empty, reader);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(DocumentWriterTest, document1) {
	testDocumentWrite(documentSample_1, writer);
}

TEST_F(DocumentWriterTest, document2) {
	testDocumentWrite(documentSample_2, writer);
}

TEST_F(DocumentWriterTest, documentEmpty) {
	testDocumentWrite(documentSample_empty, writer);
}
