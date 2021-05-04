#include <gmock/gmock.h>
#include <cstdio>
#include <fstream>
#include "Exceptions/IOException.h"
#include "Utils/CsvReaderImpl.h"

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

TEST(CsvReaderFactoryTest, read) {
	char filename[L_tmpnam];
	tmpnam(filename);

	{
		std::ofstream ofs(filename);
		ofs << "1,2,3,4" << std::endl;
		ofs << "5,6,7" << std::endl;
		ofs.close();
	}

	CsvReaderFactoryImpl csvReaderFactory(filename);
	auto csvReader = csvReaderFactory.create();

	ASSERT_THAT(csvReader, Not(IsNull()));
	EXPECT_THAT(csvReader->read(), Pointee(std::vector<std::string>{ "1", "2", "3", "4"}));
	EXPECT_THAT(csvReader->read(), Pointee(std::vector<std::string>{ "5", "6", "7"}));
	EXPECT_THAT(csvReader->read(), IsNull());
}

TEST(CsvReaderFactoryTest, readInvalid) {
	char filename[L_tmpnam];
	tmpnam(filename);

	CsvReaderFactoryImpl csvReaderFactory(filename);

	EXPECT_THROW(csvReaderFactory.create(), IOException);
}
