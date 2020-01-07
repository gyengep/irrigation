#include <gmock/gmock.h>
#include <sstream>
#include "Mocks/MockCsvWriter.h"
#include "Mocks/MockTemperatureHistory.h"
#include "Temperature/TemperatureHistoryPersister.h"
#include "Utils/CsvWriterImpl.h"
#include "Utils/TimeConversion.h"

using namespace std;
using namespace testing;

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

TEST(TemperatureHistoryPersisterTest, createNewFile) {
	auto mockCsvWriter = make_shared<MockCsvWriter>();
	auto mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	shared_ptr<ostream> output = make_shared<ostringstream>();

	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "Date", "MinTemperature", "MaxTemperature", "AvgTemperature" })).Times(1);
	EXPECT_CALL(*mockCsvWriter, stream()).Times(AnyNumber()).WillRepeatedly(ReturnRef(output));

	TemperatureHistoryPersister temperatureHistoryPersister(
			mockTemperatureHistory,
			mockCsvWriter
		);
}

TEST(TemperatureHistoryPersisterTest, createNewFileAndAppend) {
	auto mockCsvWriter = make_shared<MockCsvWriter>();
	auto mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	shared_ptr<ostream> output = make_shared<ostringstream>();

	const time_t rawTime = fromLocalTime(2019, 6, 12, 22, 0, 0);
	const TemperatureHistory::Values result(10, 20, 15);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(rawTime, rawTime + chrono::seconds(60).count())).Times(1).WillOnce(Return(result));
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "Date", "MinTemperature", "MaxTemperature", "AvgTemperature" })).Times(1);
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "2019.06.12 22:00", "10.0", "20.0", "15.0"})).Times(1);
	EXPECT_CALL(*mockCsvWriter, stream()).Times(AnyNumber()).WillRepeatedly(ReturnRef(output));

	TemperatureHistoryPersister temperatureHistoryPersister(
			mockTemperatureHistory,
			mockCsvWriter
		);

	temperatureHistoryPersister.saveHistory(rawTime, rawTime + chrono::seconds(60).count());
}

TEST(TemperatureHistoryPersisterTest, appendFile) {
	auto mockCsvWriter = make_shared<MockCsvWriter>();
	auto mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	shared_ptr<ostream> output = make_shared<ostringstream>();
	(*output) << "Date,MinTemperature,MaxTemperature,AvgTemperature" << endl;

	const time_t rawTime = fromLocalTime(2019, 6, 12, 22, 0, 0);
	const TemperatureHistory::Values result(10, 20, 15);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(rawTime, rawTime + chrono::seconds(60).count())).Times(1).WillOnce(Return(result));
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "2019.06.12 22:00", "10.0", "20.0", "15.0"})).Times(1);
	EXPECT_CALL(*mockCsvWriter, stream()).Times(AnyNumber()).WillRepeatedly(ReturnRef(output));

	TemperatureHistoryPersister temperatureHistoryPersister(
			mockTemperatureHistory,
			mockCsvWriter
		);

	temperatureHistoryPersister.saveHistory(rawTime, rawTime + chrono::seconds(60).count());
}

TEST(TemperatureHistoryPersisterTest, onTimer) {
	auto mockCsvWriter = make_shared<MockCsvWriter>();
	auto mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	shared_ptr<ostream> output = make_shared<ostringstream>();

	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "Date", "MinTemperature", "MaxTemperature", "AvgTemperature" })).Times(1);
	EXPECT_CALL(*mockCsvWriter, stream()).Times(AnyNumber()).WillRepeatedly(ReturnRef(output));

	TemperatureHistoryPersister temperatureHistoryPersister(
			mockTemperatureHistory,
			mockCsvWriter
		);

	temperatureHistoryPersister.startTimer(chrono::minutes(10));
	EXPECT_NO_THROW(temperatureHistoryPersister.onTimer());
	temperatureHistoryPersister.stopTimer();
}
