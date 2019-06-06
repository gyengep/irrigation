#include <gmock/gmock.h>
#include <fstream>
#include <list>
#include <sstream>
#include "Mocks/MockCsvWriter.h"
#include "Hardware/Temperature/TemperaturePersister.h"
#include "Utils/CsvWriterImpl.h"
#include "Utils/TimeConversion.h"

using namespace std;
using namespace testing;

using ::testing::Sequence;

///////////////////////////////////////////////////////////////////////////////

class MockCsvWriterFactory : public CsvWriterFactory {
	const shared_ptr<CsvWriter> csvWriter;

public:
	MockCsvWriterFactory(const shared_ptr<CsvWriter>& csvWriter) : csvWriter(csvWriter) {}
	virtual shared_ptr<CsvWriter> create(const shared_ptr<ostream>& output) override {
		return csvWriter;
	}
};

///////////////////////////////////////////////////////////////////////////////

string toTimeStr(int year, int month, int day, int hour, int min) {
	tm timeinfo = toCalendarTime(year, month, day, hour, min, 0);
	char buffer[32];
	strftime(buffer, 32, "%Y.%m.%d %H:%M", &timeinfo);
	return string(buffer);

}

time_t toTime(int year, int month, int day, int hour, int min) {
	tm timeinfo = toCalendarTime(year, month, day, hour, min, 0);
	return mktime(&timeinfo);

}

TEST(TemperaturePersisterTest, header) {
	auto mockCsvWriter = make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "Date", "Temperature" })).Times(1);

	TemperaturePersister temperaturePersister(tmpnam(nullptr), make_shared<MockCsvWriterFactory>(mockCsvWriter));
}

TEST(TemperaturePersisterTest, append) {
	Sequence seq;

	auto mockCsvWriter = make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "Date", "Temperature" })).Times(1).InSequence(seq);

	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 12, 2, 22), "52.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 13, 3, 23), "53.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 14, 4, 22), "54.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 15, 5, 22), "55.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 3, 16, 6, 22), "56.0" })).Times(1).InSequence(seq);

	TemperaturePersister temperaturePersister(tmpnam(nullptr), make_shared<MockCsvWriterFactory>(mockCsvWriter));
	temperaturePersister.append(toTime(2019, 2, 12, 2, 22), 52);
	temperaturePersister.append(toTime(2019, 2, 13, 3, 23), 53);
	temperaturePersister.append(toTime(2019, 2, 14, 4, 22), 54);
	temperaturePersister.append(toTime(2019, 2, 15, 5, 22), 55);
	temperaturePersister.append(toTime(2019, 3, 16, 6, 22), 56);
}

TEST(TemperaturePersisterTest, saveFromDestructor) {
	Sequence seq;

	auto mockCsvWriter = make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "Date", "Temperature" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 12, 2, 22), "52.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 13, 3, 23), "53.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 14, 4, 22), "54.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 15, 5, 22), "55.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 3, 16, 6, 22), "56.0" })).Times(1).InSequence(seq);

	TemperaturePersister temperaturePersister(tmpnam(nullptr), make_shared<MockCsvWriterFactory>(mockCsvWriter));
	temperaturePersister.append(toTime(2019, 2, 12, 2, 22), 52);
	temperaturePersister.append(toTime(2019, 2, 13, 3, 23), 53);
	temperaturePersister.append(toTime(2019, 2, 14, 4, 22), 54);
	temperaturePersister.append(toTime(2019, 2, 15, 5, 22), 55);
	temperaturePersister.append(toTime(2019, 3, 16, 6, 22), 56);
}

TEST(TemperaturePersisterTest, saveAndAppend) {
	Sequence seq;

	auto mockCsvWriter = make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "Date", "Temperature" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 12, 2, 22), "52.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 13, 3, 23), "53.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 14, 4, 22), "54.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 2, 15, 5, 22), "55.0" })).Times(1).InSequence(seq);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ toTimeStr(2019, 3, 16, 6, 22), "56.0" })).Times(1).InSequence(seq);

	TemperaturePersister temperaturePersister(tmpnam(nullptr), make_shared<MockCsvWriterFactory>(mockCsvWriter));
	temperaturePersister.append(toTime(2019, 2, 12, 2, 22), 52);
	temperaturePersister.append(toTime(2019, 2, 13, 3, 23), 53);
	temperaturePersister.append(toTime(2019, 2, 14, 4, 22), 54);

	temperaturePersister.persistData();
	temperaturePersister.append(toTime(2019, 2, 15, 5, 22), 55);
	temperaturePersister.append(toTime(2019, 3, 16, 6, 22), 56);
}

TEST(TemperaturePersisterTest, createFile) {
	const char* fileName = tmpnam(nullptr);

	{
		TemperaturePersister temperaturePersister(fileName, make_shared<CsvWriterImplFactory>());
		temperaturePersister.append(toTime(2019, 2, 12, 2, 22), 52);
		temperaturePersister.persistData();

		ifstream input(fileName);
		vector<char> inputData(
		         (istreambuf_iterator<char>(input)),
		         (istreambuf_iterator<char>()));

		EXPECT_THAT(string(inputData.data(), inputData.size()), "Date,Temperature\n2019.02.12 02:22,52.0\n");
	}

	{
		TemperaturePersister temperaturePersister(fileName, make_shared<CsvWriterImplFactory>());
		temperaturePersister.append(toTime(2020, 3, 13, 4, 33), 26);
		temperaturePersister.persistData();

		ifstream input(fileName);
		vector<char> inputData(
		         (istreambuf_iterator<char>(input)),
		         (istreambuf_iterator<char>()));

		EXPECT_THAT(string(inputData.data(), inputData.size()), "Date,Temperature\n2019.02.12 02:22,52.0\n2020.03.13 04:33,26.0\n");
	}
}
