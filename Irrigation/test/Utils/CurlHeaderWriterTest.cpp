#include <gmock/gmock.h>
#include "Utils/CurlHeaderWriter.h"
#include "Exceptions/Exceptions.h"

using namespace std;
using namespace testing;


TEST(CurlHeaderWriterTest, invalid) {
	CurlHeaderWriter curlStringWriter;
	char ptr[100];

	EXPECT_NO_THROW(CurlHeaderWriter::headerFunction(ptr, 5, 1, &curlStringWriter));
	EXPECT_THROW(CurlHeaderWriter::headerFunction(nullptr, 5, 1, &curlStringWriter), IllegalArgumentException);
	EXPECT_THROW(CurlHeaderWriter::headerFunction(ptr, 5, 1, nullptr), IllegalArgumentException);

	EXPECT_THAT(CurlHeaderWriter::headerFunction(ptr, 0, 1, &curlStringWriter), Eq(0));
	EXPECT_THAT(CurlHeaderWriter::headerFunction(ptr, 5, 0, &curlStringWriter), Eq(0));
}

TEST(CurlHeaderWriterTest, writeEmpty) {
	const size_t BufferSize = 100;
	char buffer[BufferSize];

	CurlHeaderWriter curlStringWriter;

	CurlHeaderWriter::headerFunction(buffer, 1, 0, &curlStringWriter);

	EXPECT_THAT(curlStringWriter.getHeaders(), IsEmpty());
}

TEST(CurlHeaderWriterTest, writeOne) {
	char buffer[] = "0123456789";

	CurlHeaderWriter curlStringWriter;

	CurlHeaderWriter::headerFunction(buffer, 1, 5, &curlStringWriter);

	EXPECT_THAT(curlStringWriter.getHeaders(), ContainerEq(list<string>{"01234"}));
}

TEST(CurlHeaderWriterTest, writeMore) {
	char buffer[] = "0123456789";

	CurlHeaderWriter curlStringWriter;

	CurlHeaderWriter::headerFunction(buffer, 1, 3, &curlStringWriter);
	CurlHeaderWriter::headerFunction(buffer + 3, 1, 3, &curlStringWriter);
	CurlHeaderWriter::headerFunction(buffer + 6, 1, 3, &curlStringWriter);

	EXPECT_THAT(curlStringWriter.getHeaders(), ContainerEq(list<string>{"012", "345", "678"}));
}

TEST(CurlHeaderWriterTest, writeMoreBytes) {
	char buffer[] = "0123456789";

	CurlHeaderWriter curlStringWriter;

	CurlHeaderWriter::headerFunction(buffer, 2, 3, &curlStringWriter);

	EXPECT_THAT(curlStringWriter.getHeaders(), ContainerEq(list<string>{"012345"}));
}
