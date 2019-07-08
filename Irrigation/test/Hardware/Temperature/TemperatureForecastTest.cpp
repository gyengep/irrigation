#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureException.h"
#include "Hardware/Temperature/TemperatureForecast.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

class MockTemperatureForecastProvider : public TemperatureForecastProvider {
public:
	MOCK_CONST_METHOD0(getForecast, list<ValuesWithTimes>());
};

///////////////////////////////////////////////////////////////////////////////

static const list<TemperatureForecastProvider::ValuesWithTimes> valueList1 {
	TemperatureForecastProvider::ValuesWithTimes(10, 20, 12, 14),
	TemperatureForecastProvider::ValuesWithTimes(20, 30, 16, 18),
	TemperatureForecastProvider::ValuesWithTimes(30, 40, 22, 28)
};

static const list<TemperatureForecastProvider::ValuesWithTimes> valueList2 {
	TemperatureForecastProvider::ValuesWithTimes(50, 60, 22, 24),
	TemperatureForecastProvider::ValuesWithTimes(60, 70, 26, 28),
	TemperatureForecastProvider::ValuesWithTimes(70, 80, 32, 38)
};

static const list<TemperatureForecastProvider::ValuesWithTimes> invalidList1 {
	TemperatureForecastProvider::ValuesWithTimes(10, 20, 12, 14),
	TemperatureForecastProvider::ValuesWithTimes(20, 15, 16, 18),
	TemperatureForecastProvider::ValuesWithTimes(15, 40, 22, 28)
};

static const list<TemperatureForecastProvider::ValuesWithTimes> invalidList2 {
	TemperatureForecastProvider::ValuesWithTimes(10, 20, 12, 14),
	TemperatureForecastProvider::ValuesWithTimes(25, 30, 16, 18),
	TemperatureForecastProvider::ValuesWithTimes(30, 40, 22, 28)
};

TEST(TemperatureForecastTest, checkValueList) {
	EXPECT_NO_THROW(TemperatureForecast::checkValueList(valueList1));
	EXPECT_NO_THROW(TemperatureForecast::checkValueList(valueList2));
}

TEST(TemperatureForecastTest, checkValueListInvalid) {
	EXPECT_THROW(TemperatureForecast::checkValueList(invalidList1), TemperatureException);
	EXPECT_THROW(TemperatureForecast::checkValueList(invalidList2), TemperatureException);
}

TEST(TemperatureForecastTest, updateCache) {
	auto mockProvider = make_shared<MockTemperatureForecastProvider>();
	EXPECT_CALL(*mockProvider, getForecast()).Times(1).
			WillOnce(Return(valueList1));

	TemperatureForecast temperatureForecast(mockProvider);

	temperatureForecast.updateCache();
	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList1));
}

TEST(TemperatureForecastTest, updateCacheInvalid) {
	auto mockProvider = make_shared<MockTemperatureForecastProvider>();
	EXPECT_CALL(*mockProvider, getForecast()).Times(1).WillOnce(Return(invalidList1));

	TemperatureForecast temperatureForecast(mockProvider);
	temperatureForecast.updateCache();

	EXPECT_THAT(temperatureForecast.getContainer(), IsEmpty());
}

TEST(TemperatureForecastTest, updateCacheTwoTimes) {
	auto mockProvider = make_shared<MockTemperatureForecastProvider>();
	EXPECT_CALL(*mockProvider, getForecast()).Times(2).
			WillOnce(Return(valueList1)).
			WillOnce(Return(valueList2));

	TemperatureForecast temperatureForecast(mockProvider);

	temperatureForecast.updateCache();
	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList1));

	temperatureForecast.updateCache();
	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList2));
}

TEST(TemperatureForecastTest, updateCacheTwoTimesInvalid) {
	auto mockProvider = make_shared<MockTemperatureForecastProvider>();
	EXPECT_CALL(*mockProvider, getForecast()).Times(2).
			WillOnce(Return(valueList1)).
			WillOnce(Return(invalidList1));

	TemperatureForecast temperatureForecast(mockProvider);

	temperatureForecast.updateCache();
	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList1));

	temperatureForecast.updateCache();
	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList1));
}

TEST(TemperatureForecastTest, getForecastValues) {
	const list<TemperatureForecastProvider::ValuesWithTimes> expectedList {
		TemperatureForecastProvider::ValuesWithTimes(10, 20, 12, 14),
		TemperatureForecastProvider::ValuesWithTimes(20, 30, 16, 18),
		TemperatureForecastProvider::ValuesWithTimes(30, 40, 20, 22),
		TemperatureForecastProvider::ValuesWithTimes(40, 50, 24, 26),
		TemperatureForecastProvider::ValuesWithTimes(50, 60, 28, 30)
	};

	auto mockProvider = make_shared<MockTemperatureForecastProvider>();
	EXPECT_CALL(*mockProvider, getForecast()).Times(1).WillOnce(Return(expectedList));

	TemperatureForecast temperatureForecast(mockProvider);
	temperatureForecast.updateCache();

	EXPECT_THAT(temperatureForecast.getForecastValues(20, 40), TemperatureForecast::Values(16, 22));
	EXPECT_THAT(temperatureForecast.getForecastValues(15, 40), TemperatureForecast::Values(12, 22));
	EXPECT_THAT(temperatureForecast.getForecastValues(15, 45), TemperatureForecast::Values(12, 26));
	EXPECT_THAT(temperatureForecast.getForecastValues(20, 45), TemperatureForecast::Values(16, 26));
}

TEST(TemperatureForecastTest, getForecastOutOfBounds) {
	const list<TemperatureForecastProvider::ValuesWithTimes> expectedList {
		TemperatureForecastProvider::ValuesWithTimes(10, 20, 12, 14),
		TemperatureForecastProvider::ValuesWithTimes(20, 30, 16, 18),
		TemperatureForecastProvider::ValuesWithTimes(30, 40, 20, 22),
		TemperatureForecastProvider::ValuesWithTimes(40, 50, 24, 26),
		TemperatureForecastProvider::ValuesWithTimes(50, 60, 28, 30)
	};

	auto mockProvider = make_shared<MockTemperatureForecastProvider>();
	EXPECT_CALL(*mockProvider, getForecast()).Times(1).WillOnce(Return(expectedList));

	TemperatureForecast temperatureForecast(mockProvider);
	temperatureForecast.updateCache();

	EXPECT_ANY_THROW(temperatureForecast.getForecastValues(60, 70));
	EXPECT_ANY_THROW(temperatureForecast.getForecastValues(5, 10));
}

TEST(TemperatureForecastTest, onTimer) {
	auto mockProvider = make_shared<MockTemperatureForecastProvider>();

	EXPECT_CALL(*mockProvider, getForecast()).Times(1).
			WillOnce(Return(valueList1));

	TemperatureForecast temperatureForecast(mockProvider);

	EXPECT_NO_THROW(temperatureForecast.onTimer());
}
