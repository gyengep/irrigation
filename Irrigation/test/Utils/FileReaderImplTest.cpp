#include <gmock/gmock.h>
#include <cstdio>
#include "Exceptions/Exceptions.h"
#include "Utils/FileReaderImpl.h"

using namespace std;
using namespace testing;


TEST(FileReaderTest, fileNotExist) {
	FileReaderImpl fileReader(tmpnam(nullptr));
	EXPECT_THROW(fileReader.read(), FileNotFoundException);
}

TEST(FileReaderTest, read) {
	const string text("123456789");
	const string name = tmpnam(nullptr);

	FILE* f = fopen(name.c_str(), "wb");
	fputs(text.c_str(), f);
	fclose(f);

	FileReaderImpl fileReader(name);
	EXPECT_THAT(fileReader.read(), Eq(text));

	remove(name.c_str());
}
