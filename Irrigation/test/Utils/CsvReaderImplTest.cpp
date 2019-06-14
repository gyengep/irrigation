#include <gmock/gmock.h>
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"

using namespace std;
using namespace testing;


TEST(CsvReaderImplTest, readEmpty) {
	CsvReaderImpl csvReader(make_shared<istringstream>(""));
	EXPECT_THAT(csvReader.read(), IsNull());
}

TEST(CsvReaderImplTest, readOneLine) {
	CsvReaderImpl csvReader(make_shared<istringstream>("1,2,3,4"));
	EXPECT_THAT(csvReader.read(), Pointee(vector<string>{ "1", "2", "3", "4" }));
	EXPECT_THAT(csvReader.read(), IsNull());
}

TEST(CsvReaderImplTest, readMoreLine) {
	CsvReaderImpl csvReader(make_shared<istringstream>("1,2,3,4\n5,6,7"));
	EXPECT_THAT(csvReader.read(), Pointee(vector<string>{ "1", "2", "3", "4" }));
	EXPECT_THAT(csvReader.read(), Pointee(vector<string>{ "5", "6", "7" }));
	EXPECT_THAT(csvReader.read(), IsNull());
}

TEST(CsvReaderImplTest, readerWriter) {
	const vector<string> line1 { "1", "2", "3" };
	const vector<string> line2 { "abc", "DEFG", "12345" };
	const vector<string> line3 { "Asdf", "Yxcvbn", "Rrtzu" };

	auto output = make_shared<ostringstream>();

	{
		CsvWriterImpl csvWriter(output);
		csvWriter.append(line1);
		csvWriter.append(line2);
		csvWriter.append(line3);
	}

	CsvReaderImpl csvReader(make_shared<istringstream>(output->str()));

	EXPECT_THAT(csvReader.read(), Pointee(line1));
	EXPECT_THAT(csvReader.read(), Pointee(line2));
	EXPECT_THAT(csvReader.read(), Pointee(line3));
	EXPECT_THAT(csvReader.read(), IsNull());
}
