#include <gmock/gmock.h>
#include "Temperature/TemperatureException.h"
#include "Temperature/Impl/TemperatureForecastImpl.h"
#include "Mocks/MockTemperatureForecastProvider.h"

using namespace std;
using namespace chrono;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

static const list<TemperatureForecastProvider::ValuesWithTimes> valueList1 {
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(10), DateTime::create(20), 12, 14),
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(20), DateTime::create(30), 16, 18),
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(30), DateTime::create(40), 22, 28)
};

static const list<TemperatureForecastProvider::ValuesWithTimes> valueList2 {
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(50), DateTime::create(60), 22, 24),
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(60), DateTime::create(70), 26, 28),
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(70), DateTime::create(80), 32, 38)
};

static const list<TemperatureForecastProvider::ValuesWithTimes> invalidList1 {
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(10), DateTime::create(20), 12, 14),
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(20), DateTime::create(15), 16, 18),
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(15), DateTime::create(40), 22, 28)
};

static const list<TemperatureForecastProvider::ValuesWithTimes> invalidList2 {
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(10), DateTime::create(20), 12, 14),
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(25), DateTime::create(30), 16, 18),
	TemperatureForecastProvider::ValuesWithTimes(DateTime::create(30), DateTime::create(40), 22, 28)
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
		TemperatureForecastProvider::ValuesWithTimes(DateTime::create(10), DateTime::create(20), 12, 14),
		TemperatureForecastProvider::ValuesWithTimes(DateTime::create(20), DateTime::create(30), 16, 18),
		TemperatureForecastProvider::ValuesWithTimes(DateTime::create(30), DateTime::create(40), 20, 22),
		TemperatureForecastProvider::ValuesWithTimes(DateTime::create(40), DateTime::create(50), 24, 26),
		TemperatureForecastProvider::ValuesWithTimes(DateTime::create(50), DateTime::create(60), 28, 30)
	};

	auto mockTemperatureForecastProvider = make_shared<MockTemperatureForecastProvider>();

	EXPECT_CALL(*mockTemperatureForecastProvider, getForecastProviderName()).Times(AnyNumber());
	EXPECT_CALL(*mockTemperatureForecastProvider, readTemperatureForecast()).
			Times(1).
			WillOnce(Return(expectedList));

	TemperatureForecastImpl temperatureForecast(mockTemperatureForecastProvider);
	temperatureForecast.updateCache();

	EXPECT_THAT(temperatureForecast.getTemperatureForecast(DateTime::create(20), DateTime::create(40)), TemperatureForecast::Values(16, 22));
	EXPECT_THAT(temperatureForecast.getTemperatureForecast(DateTime::create(15), DateTime::create(40)), TemperatureForecast::Values(12, 22));
	EXPECT_THAT(temperatureForecast.getTemperatureForecast(DateTime::create(15), DateTime::create(45)), TemperatureForecast::Values(12, 26));
	EXPECT_THAT(temperatureForecast.getTemperatureForecast(DateTime::create(20), DateTime::create(45)), TemperatureForecast::Values(16, 26));
}

TEST(TemperatureForecastImplTest, getForecastOutOfBounds) {
	const list<TemperatureForecastProvider::ValuesWithTimes> expectedList {
		TemperatureForecastProvider::ValuesWithTimes(DateTime::create(10), DateTime::create(20), 12, 14),
		TemperatureForecastProvider::ValuesWithTimes(DateTime::create(20), DateTime::create(30), 16, 18),
		TemperatureForecastProvider::ValuesWithTimes(DateTime::create(30), DateTime::create(40), 20, 22),
		TemperatureForecastProvider::ValuesWithTimes(DateTime::create(40), DateTime::create(50), 24, 26),
		TemperatureForecastProvider::ValuesWithTimes(DateTime::create(50), DateTime::create(60), 28, 30)
	};

	auto mockTemperatureForecastProvider = make_shared<MockTemperatureForecastProvider>();

	EXPECT_CALL(*mockTemperatureForecastProvider, getForecastProviderName()).Times(AnyNumber());
	EXPECT_CALL(*mockTemperatureForecastProvider, readTemperatureForecast()).
			Times(1).
			WillOnce(Return(expectedList));

	TemperatureForecastImpl temperatureForecast(mockTemperatureForecastProvider);
	temperatureForecast.updateCache();

	EXPECT_THROW(temperatureForecast.getTemperatureForecast(DateTime::create(60), DateTime::create(70)), TemperatureException);
	EXPECT_THROW(temperatureForecast.getTemperatureForecast(DateTime::create(5), DateTime::create(10)), TemperatureException);
}

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureForecastImplTest, timer) {
	auto mockTemperatureForecastProvider = make_shared<MockTemperatureForecastProvider>();

	EXPECT_CALL(*mockTemperatureForecastProvider, getForecastProviderName()).Times(AnyNumber());
	EXPECT_CALL(*mockTemperatureForecastProvider, readTemperatureForecast()).
			Times(3).
			WillOnce(Return(valueList1)).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Return(valueList2));

	TemperatureForecastImpl temperatureForecast(mockTemperatureForecastProvider);

	temperatureForecast.start(std::chrono::seconds(1), std::vector<std::chrono::milliseconds>{ std::chrono::milliseconds(500) });

	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList1));
	std::this_thread::sleep_for(std::chrono::seconds(2));
	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(valueList2));

	temperatureForecast.stop();
}
