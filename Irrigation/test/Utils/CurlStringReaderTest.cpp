#include <gmock/gmock.h>
#include "Utils/CurlStringReader.h"
#include "Exceptions/Exceptions.h"

using namespace std;
using namespace testing;


TEST(CurlStringReaderTest, invalid) {
	CurlStringReader curlStringReader("0123456789");
	char ptr[100];

	EXPECT_NO_THROW(CurlStringReader::readFunction(ptr, 5, 1, &curlStringReader));
	EXPECT_THROW(CurlStringReader::readFunction(nullptr, 5, 1, &curlStringReader), IllegalArgumentException);
	EXPECT_THROW(CurlStringReader::readFunction(ptr, 5, 1, nullptr), IllegalArgumentException);

	EXPECT_THAT(CurlStringReader::readFunction(ptr, 0, 1, &curlStringReader), Eq(0));
	EXPECT_THAT(CurlStringReader::readFunction(ptr, 5, 0, &curlStringReader), Eq(0));
}

TEST(CurlStringReaderTest, readEmpty) {
	CurlStringReader curlStringReader("");
	const size_t BufferSize = 100;
	char buffer[BufferSize];

	EXPECT_THAT(CurlStringReader::readFunction(buffer, 1, BufferSize, &curlStringReader), Eq(0));
}

TEST(CurlStringReaderTest, readShort) {
	const string text = "0123456789";

	CurlStringReader curlStringReader(text);
	const size_t BufferSize = 100;
	char buffer1[BufferSize] = {0};
	char buffer2[BufferSize] = {0};

	EXPECT_THAT(CurlStringReader::readFunction(buffer1, 1, BufferSize, &curlStringReader), Eq(text.length()));
	EXPECT_THAT(string(buffer1, text.length()), Eq(text));

	EXPECT_THAT(CurlStringReader::readFunction(buffer2, 1, BufferSize, &curlStringReader), Eq(0));
}

TEST(CurlStringReaderTest, readLong) {
	const string text = "0123456789";

	CurlStringReader curlStringReader(text);
	const size_t BufferSize = 3;
	char buffer1[BufferSize] = {0};
	char buffer2[BufferSize] = {0};
	char buffer3[BufferSize] = {0};
	char buffer4[BufferSize] = {0};
	char buffer5[BufferSize] = {0};

	EXPECT_THAT(CurlStringReader::readFunction(buffer1, 1, BufferSize, &curlStringReader), Eq(3));
	EXPECT_THAT(string(buffer1, 3), Eq("012"));

	EXPECT_THAT(CurlStringReader::readFunction(buffer2, 1, BufferSize, &curlStringReader), Eq(3));
	EXPECT_THAT(string(buffer2, 3), Eq("345"));

	EXPECT_THAT(CurlStringReader::readFunction(buffer3, 1, BufferSize, &curlStringReader), Eq(3));
	EXPECT_THAT(string(buffer3, 3), Eq("678"));

	EXPECT_THAT(CurlStringReader::readFunction(buffer4, 1, BufferSize, &curlStringReader), Eq(1));
	EXPECT_THAT(string(buffer4, 1), Eq("9"));

	EXPECT_THAT(CurlStringReader::readFunction(buffer5, 1, BufferSize, &curlStringReader), Eq(0));
}

TEST(CurlStringReaderTest, readMoreBytes) {
	const string text = "0123456789";

	CurlStringReader curlStringReader(text);
	const size_t BufferSize = 100;
	char buffer1[BufferSize] = {0};
	char buffer2[BufferSize] = {0};
	char buffer3[BufferSize] = {0};
	const size_t dataSize = 3;

	EXPECT_THAT(CurlStringReader::readFunction(buffer1, dataSize, 2, &curlStringReader), Eq(2));
	EXPECT_THAT(string(buffer1, 6), Eq("012345"));

	EXPECT_THAT(CurlStringReader::readFunction(buffer2, dataSize, 2, &curlStringReader), Eq(1));
	EXPECT_THAT(string(buffer2, 3), Eq("678"));

	EXPECT_THAT(CurlStringReader::readFunction(buffer3, dataSize, 2, &curlStringReader), Eq(0));
}

