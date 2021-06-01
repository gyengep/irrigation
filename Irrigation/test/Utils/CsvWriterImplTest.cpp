#include <gmock/gmock.h>
#include <algorithm>
#include <fstream>
#include <iterator>
#include "Exceptions/IOException.h"
#include "Utils/CsvWriterImpl.h"

using namespace std;
using namespace testing;


TEST(CsvWriterImplTest, writeOneLine) {
	auto oss = make_shared<ostringstream>();
	CsvWriterImpl csvWriter(oss);

	csvWriter.append(vector<string>{ "1", "2", "3", "4" });

	EXPECT_THAT(oss->str(), Eq("1,2,3,4\n"));
}

TEST(CsvWriterImplTest, writeMoreLine) {
	auto oss = make_shared<ostringstream>();
	CsvWriterImpl csvWriter(oss);

	csvWriter.append(vector<string>{ "1", "2", "3", "4" });
	csvWriter.append(vector<string>{ "5", "6", "7" });

	EXPECT_THAT(oss->str(), Eq("1,2,3,4\n5,6,7\n"));
}

TEST(CsvWriterFactoryTest, write) {
	char filename[L_tmpnam];
	tmpnam(filename);

	CsvWriterFactoryImpl csvWriterFactory(filename);
	auto csvWriter = csvWriterFactory.create();

	ASSERT_THAT(csvWriter, Not(IsNull()));

	csvWriter->append(std::vector<std::string>{ "1", "2", "3", "4"});
	csvWriter->append(std::vector<std::string>{ "5", "6", "7"});

	{
		std::ifstream t(filename);
		std::string str((std::istreambuf_iterator<char>(t)),
		                 std::istreambuf_iterator<char>());

		EXPECT_THAT(str, Eq("1,2,3,4\n5,6,7\n"));
	}
}

TEST(CsvWriterFactoryTest, writeInvalid) {
	CsvWriterFactoryImpl csvWriterFactory("fadzfauxuax/123/456/45132/63/5");
	EXPECT_THROW(csvWriterFactory.create(), IOException);
}
