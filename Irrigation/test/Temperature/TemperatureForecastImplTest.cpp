#include <gmock/gmock.h>
#include "Temperature/TemperatureException.h"
#include "Temperature/TemperatureForecastImpl.h"
#include "Mocks/MockTemperatureForecastProvider.h"

using namespace std;
using namespace chrono;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

static const list<TemperatureForecastProvider::ValuesWithTimes> valueList1 {
	TemperatureForecastProvider::ValuesWithTimes(DateTime(10), DateTime(20), 12, 14),
	TemperatureForecastProvider::ValuesWithTimes(DateTime(20), DateTime(30), 16, 18),
	TemperatureForecastProvider::ValuesWithTimes(DateTime(30), DateTime(40), 22, 28)
};

static const list<TemperatureForecastProvider::ValuesWithTimes> valueList2 {
	TemperatureForecastProvider::ValuesWithTimes(DateTime(50), DateTime(60), 22, 24),
	TemperatureForecastProvider::ValuesWithTimes(DateTime(60), DateTime(70), 26, 28),
	TemperatureForecastProvider::ValuesWithTimes(DateTime(70), DateTime(80), 32, 38)
};

static const list<TemperatureForecastProvider::ValuesWithTimes> invalidList1 {
	TemperatureForecastProvider::ValuesWithTimes(DateTime(10), DateTime(20), 12, 14),
	TemperatureForecastProvider::ValuesWithTimes(DateTime(20), DateTime(15), 16, 18),
	TemperatureForecastProvider::ValuesWithTimes(DateTime(15), DateTime(40), 22, 28)
};

static const list<TemperatureForecastProvider::ValuesWithTimes> invalidList2 {
	TemperatureForecastProvider::ValuesWithTimes(DateTime(10), DateTime(20), 12, 14),
	TemperatureForecastProvider::ValuesWithTimes(DateTime(25), DateTime(30), 16, 18),
	TemperatureForecastProvider::ValuesWithTimes(DateTime(30), DateTime(40), 22, 28)
};

TEST(TemperatureForecastImplTest, checkValueList) {
	EXPECT_NO_THROW(TemperatureForecastImpl::checkValueList(valueList1));
	EXPECT_NO_THROW(TemperatureForecastImpl::checkValueList(valueList2));
}

TEST(TemperatureForecastImplTest, checkValueListInvalid) {
	EXPECT_THROW(TemperatureForecastImpl::checkValueList(invalidList1), TemperatureException);
	EXPECT_THROW(TemperatureForecastImpl::checkValueList(invalidList2), TemperatureException);
}

TEST(TemperatureForecastImplTest, updateCache) {
	auto mockTemperatureForecastProvider = make_shared<MockTemperatureForecastProvider>();

	EXPECT_CALL(*mockTemperatureForecastProvider, getForecastProviderName()).Times(AnyNumber());
	EXPECT_CALL(*mockTemperatureForecastProvider, readTemperatureForecast()).
			Times(1).
			WillOnce(Return(valueList1));

	TemperatureForecastImpl temperatureForecast(mockTemperatureForecastProvider);

	temperatureForecast.updateCache();
	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList1));
}

TEST(TemperatureForecastImplTest, updateCacheInvalid) {
	auto mockTemperatureForecastProvider = make_shared<MockTemperatureForecastProvider>();

	EXPECT_CALL(*mockTemperatureForecastProvider, getForecastProviderName()).Times(AnyNumber());
	EXPECT_CALL(*mockTemperatureForecastProvider, readTemperatureForecast()).
			Times(1).
			WillOnce(Return(invalidList1));

	TemperatureForecastImpl temperatureForecast(mockTemperatureForecastProvider);

	EXPECT_THROW(temperatureForecast.updateCache(), std::runtime_error);
	EXPECT_THAT(temperatureForecast.getContainer(), IsEmpty());
}

TEST(TemperatureForecastImplTest, updateCacheTwoTimes) {
	auto mockTemperatureForecastProvider = make_shared<MockTemperatureForecastProvider>();

	EXPECT_CALL(*mockTemperatureForecastProvider, getForecastProviderName()).Times(AnyNumber());
	EXPECT_CALL(*mockTemperatureForecastProvider, readTemperatureForecast()).
			Times(2).
			WillOnce(Return(valueList1)).
			WillOnce(Return(valueList2));

	TemperatureForecastImpl temperatureForecast(mockTemperatureForecastProvider);

	temperatureForecast.updateCache();
	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList1));

	temperatureForecast.updateCache();
	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList2));
}

TEST(TemperatureForecastImplTest, updateCacheTwoTimesInvalid) {
	auto mockTemperatureForecastProvider = make_shared<MockTemperatureForecastProvider>();

	EXPECT_CALL(*mockTemperatureForecastProvider, getForecastProviderName()).Times(AnyNumber());
	EXPECT_CALL(*mockTemperatureForecastProvider, readTemperatureForecast()).
			Times(3).
			WillOnce(Return(valueList1)).
			WillOnce(Return(invalidList1)).
			WillOnce(Return(valueList2));

	TemperatureForecastImpl temperatureForecast(mockTemperatureForecastProvider);

	EXPECT_NO_THROW(temperatureForecast.updateCache());
	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList1));

	EXPECT_THROW(temperatureForecast.updateCache(), std::runtime_error);
	EXPECT_THAT(temperatureForecast.getContainer(), IsEmpty());

	EXPECT_NO_THROW(temperatureForecast.updateCache());
	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList2));
}

TEST(TemperatureForecastImplTest, getTemperatureForecast) {
	const list<TemperatureForecastProvider::ValuesWithTimes> expectedList {
		TemperatureForecastProvider::ValuesWithTimes(DateTime(10), DateTime(20), 12, 14),
		TemperatureForecastProvider::ValuesWithTimes(DateTime(20), DateTime(30), 16, 18),
		TemperatureForecastProvider::ValuesWithTimes(DateTime(30), DateTime(40), 20, 22),
		TemperatureForecastProvider::ValuesWithTimes(DateTime(40), DateTime(50), 24, 26),
		TemperatureForecastProvider::ValuesWithTimes(DateTime(50), DateTime(60), 28, 30)
	};

	auto mockTemperatureForecastProvider = make_shared<MockTemperatureForecastProvider>();

	EXPECT_CALL(*mockTemperatureForecastProvider, getForecastProviderName()).Times(AnyNumber());
	EXPECT_CALL(*mockTemperatureForecastProvider, readTemperatureForecast()).
			Times(1).
			WillOnce(Return(expectedList));

	TemperatureForecastImpl temperatureForecast(mockTemperatureForecastProvider);
	temperatureForecast.updateCache();

	EXPECT_THAT(temperatureForecast.getTemperatureForecast(DateTime(20), DateTime(40)), TemperatureForecast::Values(16, 22));
	EXPECT_THAT(temperatureForecast.getTemperatureForecast(DateTime(15), DateTime(40)), TemperatureForecast::Values(12, 22));
	EXPECT_THAT(temperatureForecast.getTemperatureForecast(DateTime(15), DateTime(45)), TemperatureForecast::Values(12, 26));
	EXPECT_THAT(temperatureForecast.getTemperatureForecast(DateTime(20), DateTime(45)), TemperatureForecast::Values(16, 26));
}

TEST(TemperatureForecastImplTest, getForecastOutOfBounds) {
	const list<TemperatureForecastProvider::ValuesWithTimes> expectedList {
		TemperatureForecastProvider::ValuesWithTimes(DateTime(10), DateTime(20), 12, 14),
		TemperatureForecastProvider::ValuesWithTimes(DateTime(20), DateTime(30), 16, 18),
		TemperatureForecastProvider::ValuesWithTimes(DateTime(30), DateTime(40), 20, 22),
		TemperatureForecastProvider::ValuesWithTimes(DateTime(40), DateTime(50), 24, 26),
		TemperatureForecastProvider::ValuesWithTimes(DateTime(50), DateTime(60), 28, 30)
	};

	auto mockTemperatureForecastProvider = make_shared<MockTemperatureForecastProvider>();

	EXPECT_CALL(*mockTemperatureForecastProvider, getForecastProviderName()).Times(AnyNumber());
	EXPECT_CALL(*mockTemperatureForecastProvider, readTemperatureForecast()).
			Times(1).
			WillOnce(Return(expectedList));

	TemperatureForecastImpl temperatureForecast(mockTemperatureForecastProvider);
	temperatureForecast.updateCache();

	EXPECT_THROW(temperatureForecast.getTemperatureForecast(DateTime(60), DateTime(70)), TemperatureException);
	EXPECT_THROW(temperatureForecast.getTemperatureForecast(DateTime(5), DateTime(10)), TemperatureException);
}
