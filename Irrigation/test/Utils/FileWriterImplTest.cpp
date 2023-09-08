#include "FileWriterImplTest.h"

using namespace testing;


std::string FileWriterImplTest::getFileContent(const std::string& fileName) {
	const int BufferSize = 1000;
	char buffer[1000];

	FILE* f = fopen(fileName.c_str(), "rb");
	int length = fread(buffer, 1, BufferSize, f);

	if (length == BufferSize) {
		throw std::logic_error("BufferSize is too short");
	}

	fclose(f);
	return std::string(buffer, length);
}


TEST_F(FileWriterImplTest, append) {
	const std::string fileName(tmpnam(nullptr));

	const std::string expectedText1 = "line1";
	const std::string expectedText2 = "line2";

	{
		FileWriterImpl fileWriter(fileName, FileWriter::Type::APPEND);
		fileWriter.write(expectedText1);
	}

	EXPECT_THAT(expectedText1, getFileContent(fileName));

	{
		FileWriterImpl fileWriter(fileName, FileWriter::Type::APPEND);
		fileWriter.write(expectedText2);
	}

	EXPECT_THAT(expectedText1 + expectedText2, getFileContent(fileName));
}

TEST_F(FileWriterImplTest, append2) {
	const std::string fileName(tmpnam(nullptr));

	const std::string expectedText1 = "line1";
	const std::string expectedText2 = "line2";

	FileWriterImplFactory fileWriterFactory(fileName);

	fileWriterFactory.create(FileWriter::Type::APPEND)->write(expectedText1);
	EXPECT_THAT(expectedText1, getFileContent(fileName));

	fileWriterFactory.create(FileWriter::Type::APPEND)->write(expectedText2);
	EXPECT_THAT(expectedText1 + expectedText2, getFileContent(fileName));
}

TEST_F(FileWriterImplTest, truncate2) {
	const std::string fileName(tmpnam(nullptr));

	const std::string expectedText1 = "line1";
	const std::string expectedText2 = "line2";

	FileWriterImplFactory fileWriterFactory(fileName);

	fileWriterFactory.create(FileWriter::Type::TRUNCATE)->write(expectedText1);
	EXPECT_THAT(expectedText1, getFileContent(fileName));

	fileWriterFactory.create(FileWriter::Type::TRUNCATE)->write(expectedText2);
	EXPECT_THAT(expectedText2, getFileContent(fileName));
}
