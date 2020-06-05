#include <gmock/gmock.h>
#include "Utils/CurlStringWriter.h"
#include "Exceptions/Exceptions.h"

using namespace std;
using namespace testing;


TEST(CurlStringWriterTest, invalid) {
	CurlStringWriter curlStringWriter;
	char ptr[100];

	EXPECT_NO_THROW(CurlStringWriter::writeFunction(ptr, 5, 1, &curlStringWriter));
	EXPECT_THROW(CurlStringWriter::writeFunction(nullptr, 5, 1, &curlStringWriter), IllegalArgumentException);
	EXPECT_THROW(CurlStringWriter::writeFunction(ptr, 5, 1, nullptr), IllegalArgumentException);

	EXPECT_THAT(CurlStringWriter::writeFunction(ptr, 0, 1, &curlStringWriter), Eq(0));
	EXPECT_THAT(CurlStringWriter::writeFunction(ptr, 5, 0, &curlStringWriter), Eq(0));
}

TEST(CurlStringWriterTest, writeEmpty) {
	const size_t BufferSize = 100;
	char buffer[BufferSize];

	CurlStringWriter curlStringWriter;

	CurlStringWriter::writeFunction(buffer, 1, 0, &curlStringWriter);

	EXPECT_THAT(curlStringWriter.getText(), IsEmpty());
}

TEST(CurlStringWriterTest, writeShort) {
	char buffer[] = "0123456789";

	CurlStringWriter curlStringWriter;

	CurlStringWriter::writeFunction(buffer, 1, 5, &curlStringWriter);

	EXPECT_THAT(curlStringWriter.getText(), Eq("01234"));
}

TEST(CurlStringWriterTest, writeLong) {
	char buffer[] = "0123456789";

	CurlStringWriter curlStringWriter;

	CurlStringWriter::writeFunction(buffer, 1, 3, &curlStringWriter);
	CurlStringWriter::writeFunction(buffer + 3, 1, 3, &curlStringWriter);
	CurlStringWriter::writeFunction(buffer + 6, 1, 3, &curlStringWriter);

	EXPECT_THAT(curlStringWriter.getText(), Eq("012345678"));
}

TEST(CurlStringWriterTest, writeMoreBytes) {
	char buffer[] = "0123456789";

	CurlStringWriter curlStringWriter;

	CurlStringWriter::writeFunction(buffer, 2, 3, &curlStringWriter);

	EXPECT_THAT(curlStringWriter.getText(), Eq("012345"));
}
