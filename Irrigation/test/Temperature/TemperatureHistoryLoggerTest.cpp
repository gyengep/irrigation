#include <gmock/gmock.h>
#include "Mocks/MockCsvWriter.h"
#include "Mocks/MockTemperatureHistory.h"
#include "Temperature/TemperatureHistoryLogger.h"
#include "Utils/TimeConversion.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureHistoryLoggerTest, save) {
	auto mockTemperatureHistory = make_shared<MockTemperatureHistory>();
	auto mockCsvWriter = make_shared<MockCsvWriter>();
	auto mockCsvWriterFactory = make_shared<MockCsvWriterFactory>(mockCsvWriter);

	const time_t rawTime = fromLocalTime(2019, 6, 12, 22, 0, 0);
	const TemperatureHistory::Values result(10, 20, 15);

	EXPECT_CALL(*mockTemperatureHistory, getTemperatureHistory(rawTime, rawTime + chrono::seconds(60).count())).Times(1).WillOnce(Return(result));
	EXPECT_CALL(*mockCsvWriter, append(vector<string>{ "2019.06.12 22:00", "10.0", "20.0", "15.0"})).Times(1);

	TemperatureHistoryLogger temperatureHistoryLogger(
			mockTemperatureHistory,
			mockCsvWriterFactory
		);

	temperatureHistoryLogger.saveLog(rawTime, rawTime + chrono::seconds(60).count());
}

