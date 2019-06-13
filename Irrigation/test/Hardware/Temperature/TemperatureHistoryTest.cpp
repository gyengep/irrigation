#include <gmock/gmock.h>
#include <fstream>
#include <list>
#include <sstream>
#include "Mocks/MockCsvWriter.h"
#include "Mocks/MockTemperatureStatistics.h"
#include "Hardware/Temperature/TemperatureHistory.h"
#include "Utils/CsvWriterImpl.h"
#include "Utils/TimeConversion.h"

using namespace std;
using namespace testing;

using ::testing::Sequence;

const vector<string> csvHeader { "Date", "MinTemperature", "MaxTemperature", "AvgTemperature" };

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

time_t toTime(int year, int month, int day, int hour, int min, bool isdst) {
	tm timeinfo = toCalendarTime(year, month, day, hour, min, 0);
	timeinfo.tm_isdst = (isdst ? 1 : 0);
	return mktime(&timeinfo);

}

TEST(TemperatureHistoryTest, init) {
	auto mockCsvWriter = make_shared<MockCsvWriter>();
	auto mockTemperatureStatistics = make_shared<MockTemperatureStatistics>();
	shared_ptr<ostream> output = make_shared<ostringstream>();

	EXPECT_CALL(*mockTemperatureStatistics, getStatistics(_, _)).Times(0);
	EXPECT_CALL(*mockCsvWriter, append(csvHeader)).Times(1);
	EXPECT_CALL(*mockCsvWriter, stream()).Times(AnyNumber()).WillRepeatedly(ReturnRef(output));

	TemperatureHistory temperatureHistory(
			chrono::hours(1),
			mockTemperatureStatistics,
			mockCsvWriter,
			time(nullptr)
		);
}

TEST(TemperatureHistoryTest, createNewFile) {
	auto mockCsvWriter = make_shared<MockCsvWriter>();
	auto mockTemperatureStatistics = make_shared<MockTemperatureStatistics>();
	shared_ptr<ostream> output = make_shared<ostringstream>();

	const time_t currentTime = toTime(2019, 6, 12, 22, 0, true);

	EXPECT_CALL(*mockTemperatureStatistics, getStatistics(_, _)).Times(0);
	EXPECT_CALL(*mockCsvWriter, append(csvHeader)).Times(1);
	EXPECT_CALL(*mockCsvWriter, stream()).Times(AnyNumber()).WillRepeatedly(ReturnRef(output));

	TemperatureHistory temperatureHistory(
			chrono::hours(1),
			mockTemperatureStatistics,
			mockCsvWriter,
			currentTime
		);

	for (unsigned sec = 0; sec < 3600; sec++) {
		temperatureHistory.periodicUpdate(currentTime + sec);
	}
}

TEST(TemperatureHistoryTest, createNewFileAndAppend) {
	auto mockCsvWriter = make_shared<MockCsvWriter>();
	auto mockTemperatureStatistics = make_shared<MockTemperatureStatistics>();
	shared_ptr<ostream> output = make_shared<ostringstream>();

	const time_t currentTime = toTime(2019, 6, 12, 22, 0, true);

	StatisticsValues result;
	result.minTemperature = 10;
	result.maxTemperature = 20;
	result.avgTemperature = 15;

	EXPECT_CALL(*mockTemperatureStatistics, getStatistics(currentTime, currentTime + 3599)).Times(1).WillOnce(Return(result));
	EXPECT_CALL(*mockCsvWriter, append(csvHeader)).Times(1);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "2019.06.12 22:00", "10.0", "20.0", "15.0"})).Times(1);
	EXPECT_CALL(*mockCsvWriter, stream()).Times(AnyNumber()).WillRepeatedly(ReturnRef(output));

	TemperatureHistory temperatureHistory(
			chrono::hours(1),
			mockTemperatureStatistics,
			mockCsvWriter,
			currentTime
		);

	for (unsigned sec = 0; sec <= 3600; sec++) {
		temperatureHistory.periodicUpdate(currentTime + sec);
	}
}

TEST(TemperatureHistoryTest, appendFile) {
	auto mockCsvWriter = make_shared<MockCsvWriter>();
	auto mockTemperatureStatistics = make_shared<MockTemperatureStatistics>();
	shared_ptr<ostream> output = make_shared<ostringstream>();
	(*output) << "Date,MinTemperature,MaxTemperature,AvgTemperature" << endl;

	const time_t currentTime = toTime(2019, 6, 12, 22, 0, true);

	StatisticsValues result;
	result.minTemperature = 10;
	result.maxTemperature = 20;
	result.avgTemperature = 15;

	EXPECT_CALL(*mockTemperatureStatistics, getStatistics(currentTime, currentTime + 3599)).Times(1).WillOnce(Return(result));
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "2019.06.12 22:00", "10.0", "20.0", "15.0"})).Times(1);
	EXPECT_CALL(*mockCsvWriter, stream()).Times(AnyNumber()).WillRepeatedly(ReturnRef(output));

	TemperatureHistory temperatureHistory(
			chrono::hours(1),
			mockTemperatureStatistics,
			mockCsvWriter,
			currentTime
		);

	for (unsigned sec = 0; sec <= 3600; sec++) {
		temperatureHistory.periodicUpdate(currentTime + sec);
	}
}
