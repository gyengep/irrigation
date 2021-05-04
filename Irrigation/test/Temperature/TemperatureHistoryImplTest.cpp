#include <gmock/gmock.h>
#include <fstream>
#include "Mocks/MockCurrentTemperature.h"
#include "Mocks/MockTemperatureHistoryPersister.h"
#include "Temperature/TemperatureHistoryImpl.h"
#include "Temperature/TemperatureException.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"

using namespace std;
using namespace chrono;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureHistoryImplTest, init) {
	auto mockCurrentTemperature = std::make_shared<MockCurrentTemperature>();
	auto mockTemperatureHistoryPersister = std::make_shared<MockTemperatureHistoryPersister>();

	TemperatureHistoryImpl temperatureHistory(
			mockCurrentTemperature,
			mockTemperatureHistoryPersister,
			seconds(5)
		);

	EXPECT_CALL(*mockCurrentTemperature, addListener(&temperatureHistory)).Times(0);
	EXPECT_CALL(*mockCurrentTemperature, removeListener(&temperatureHistory)).Times(0);
}

TEST(TemperatureHistoryImplTest, registerUnregister) {
	auto mockCurrentTemperature = std::make_shared<MockCurrentTemperature>();
	auto mockTemperatureHistoryPersister = std::make_shared<MockTemperatureHistoryPersister>();

	TemperatureHistoryImpl temperatureHistory(
			mockCurrentTemperature,
			mockTemperatureHistoryPersister,
			seconds(5)
		);

	EXPECT_CALL(*mockCurrentTemperature, addListener(&temperatureHistory)).Times(1);
	EXPECT_CALL(*mockCurrentTemperature, removeListener(&temperatureHistory)).Times(1);

	temperatureHistory.registerToListener();
	temperatureHistory.unregisterFromListener();
}

TEST(TemperatureHistoryImplTest, onTemperatureUpdateSuccess) {
	auto mockCurrentTemperature = std::make_shared<MockCurrentTemperature>();
	auto mockTemperatureHistoryPersister = std::make_shared<MockTemperatureHistoryPersister>();

	TemperatureHistoryImpl temperatureHistory(
			mockCurrentTemperature,
			mockTemperatureHistoryPersister,
			seconds(5)
		);

	EXPECT_CALL(*mockTemperatureHistoryPersister, removeOlder(15)).Times(1);
	EXPECT_CALL(*mockTemperatureHistoryPersister, removeNewer(20)).Times(1);
	EXPECT_CALL(*mockTemperatureHistoryPersister, add(TemperatureHistoryPersister::Sample(20, 35.0f))).Times(1);
	EXPECT_CALL(*mockTemperatureHistoryPersister, getAll()).Times(0);
	EXPECT_CALL(*mockTemperatureHistoryPersister, getBetween(_, _)).Times(0);

	temperatureHistory.onTemperatureUpdated(20, 35.0f);
}

TEST(TemperatureHistoryImplTest, getHistory) {
	auto mockCurrentTemperature = std::make_shared<MockCurrentTemperature>();
	auto mockTemperatureHistoryPersister = std::make_shared<MockTemperatureHistoryPersister>();

	TemperatureHistoryImpl temperatureHistory(
			mockCurrentTemperature,
			mockTemperatureHistoryPersister,
			seconds(5)
		);

	const std::list<TemperatureHistoryPersister::Sample> result {
		TemperatureHistoryPersister::Sample(10, 15.0f),
		TemperatureHistoryPersister::Sample(11, 12.3f),
		TemperatureHistoryPersister::Sample(13, 21.0f),
		TemperatureHistoryPersister::Sample(14, 23.4f),
		TemperatureHistoryPersister::Sample(15, 19.3f),
	};

	EXPECT_CALL(*mockTemperatureHistoryPersister, getBetween(5, 20)).
			Times(3).
			WillRepeatedly(Return(result));

	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 20).max, Eq(23.4f));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 20).min, Eq(12.3f));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 20).avg, Eq(18.2f));
}

TEST(TemperatureHistoryImplTest, getHistoryNotFound) {
	auto mockCurrentTemperature = std::make_shared<MockCurrentTemperature>();
	auto mockTemperatureHistoryPersister = std::make_shared<MockTemperatureHistoryPersister>();

	TemperatureHistoryImpl temperatureHistory(
			mockCurrentTemperature,
			mockTemperatureHistoryPersister,
			seconds(5)
		);

	EXPECT_CALL(*mockTemperatureHistoryPersister, getBetween(5, 20)).
			Times(1).
			WillOnce(Return(std::list<TemperatureHistoryPersister::Sample>()));

	EXPECT_THROW(temperatureHistory.getTemperatureHistory(5, 20), TemperatureException);
}
