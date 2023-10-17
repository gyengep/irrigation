#include <gmock/gmock.h>
#include "Temperature/Impl/CurrentTemperatureImpl.h"
#include "Temperature/TemperatureException.h"
#include "Mocks/MockCurrentTemperatureProvider.h"
#include "Mocks/MockCurrentTemperatureListener.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(CurrentTemperatureImplTest, updateCache) {
	const float expectedTemperature = 35.646;

	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();

	EXPECT_CALL(*mockSensorReader, getSensorName()).Times(AnyNumber());
	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
			Times(1).
			WillOnce(Return(expectedTemperature));

	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	EXPECT_NO_THROW(temperatureSensor.updateCache());
	EXPECT_THAT(temperatureSensor.getCurrentTemperature(), Eq(expectedTemperature));
}

TEST(CurrentTemperatureImplTest, updateCacheTwoTimes) {
	const float expectedTemperature1 = 35.646;
	const float expectedTemperature2 = 21.32;

	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();

	EXPECT_CALL(*mockSensorReader, getSensorName()).Times(AnyNumber());
	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
			Times(2).
			WillOnce(Return(expectedTemperature1)).
			WillOnce(Return(expectedTemperature2));

	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	EXPECT_NO_THROW(temperatureSensor.updateCache());
	EXPECT_THAT(temperatureSensor.getCurrentTemperature(), Eq(expectedTemperature1));
	EXPECT_NO_THROW(temperatureSensor.updateCache());
	EXPECT_THAT(temperatureSensor.getCurrentTemperature(), Eq(expectedTemperature2));
}

TEST(CurrentTemperatureImplTest, updateCacheInvalid) {
	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();

	EXPECT_CALL(*mockSensorReader, getSensorName()).Times(AnyNumber());
	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
			Times(1).
			WillOnce(Throw(std::runtime_error("")));

	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	EXPECT_THROW(temperatureSensor.updateCache(), std::runtime_error);
	EXPECT_THROW(temperatureSensor.getCurrentTemperature(), TemperatureException);
}

TEST(CurrentTemperatureImplTest, updateCacheTwoTimesInvalid) {
	const float expectedTemperature1 = 35.646;
	const float expectedTemperature2 = 21.32;

	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();

	EXPECT_CALL(*mockSensorReader, getSensorName()).Times(AnyNumber());
	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
			Times(3).
			WillOnce(Return(expectedTemperature1)).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Return(expectedTemperature2));

	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	EXPECT_NO_THROW(temperatureSensor.updateCache());
	EXPECT_THAT(temperatureSensor.getCurrentTemperature(), Eq(expectedTemperature1));
	EXPECT_THROW(temperatureSensor.updateCache(), std::runtime_error);
	EXPECT_THROW(temperatureSensor.getCurrentTemperature(), TemperatureException);
	EXPECT_NO_THROW(temperatureSensor.updateCache());
	EXPECT_THAT(temperatureSensor.getCurrentTemperature(), Eq(expectedTemperature2));
}

TEST(CurrentTemperatureImplTest, listenerUpdateSuccess) {
	const float expectedTemperature = 35.646;

	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();
	MockCurrentTemperatureListener mockCurrentTemperatureListener;

	EXPECT_CALL(*mockSensorReader, getSensorName()).Times(AnyNumber());
	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
			Times(1).
			WillOnce(Return(expectedTemperature));

	EXPECT_CALL(mockCurrentTemperatureListener, onTemperatureUpdated(_, expectedTemperature)).
			Times(1);

	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	temperatureSensor.addListener(&mockCurrentTemperatureListener);
	temperatureSensor.updateCache();
	temperatureSensor.removeListener(&mockCurrentTemperatureListener);
}

TEST(CurrentTemperatureImplTest, listenerUpdateFailed) {
	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();
	MockCurrentTemperatureListener mockCurrentTemperatureListener;

	EXPECT_CALL(*mockSensorReader, getSensorName()).Times(AnyNumber());
	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
			Times(1).
			WillOnce(Throw(std::runtime_error("")));

	EXPECT_CALL(mockCurrentTemperatureListener, onTemperatureUpdated(_, _)).
			Times(0);

	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	temperatureSensor.addListener(&mockCurrentTemperatureListener);
	EXPECT_ANY_THROW(temperatureSensor.updateCache());
	temperatureSensor.removeListener(&mockCurrentTemperatureListener);
}

TEST(CurrentTemperatureImplTest, readTemperatureDuringStart) {
	const float expectedTemperature = 35.646;

	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();

	EXPECT_CALL(*mockSensorReader, getSensorName()).Times(AnyNumber());
	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
			Times(1).
			WillRepeatedly(Return(expectedTemperature));

	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	temperatureSensor.start(std::chrono::milliseconds(1000), std::vector<std::chrono::milliseconds>{ std::chrono::milliseconds(10) });
	EXPECT_THAT(temperatureSensor.getCurrentTemperature(), Eq(expectedTemperature));
	temperatureSensor.stop();
}

TEST(CurrentTemperatureImplTest, readTemperaturePeriodically_TIMING) {
	const float expectedTemperature = 35.646;

	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();

	EXPECT_CALL(*mockSensorReader, getSensorName()).Times(AnyNumber());
	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
			Times(AtLeast(3)).
			WillRepeatedly(Return(expectedTemperature));

	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	temperatureSensor.start(std::chrono::milliseconds(100), std::vector<std::chrono::milliseconds>{ std::chrono::milliseconds(10) });
	EXPECT_THAT(temperatureSensor.getCurrentTemperature(), Eq(expectedTemperature));
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	EXPECT_THAT(temperatureSensor.getCurrentTemperature(), Eq(expectedTemperature));
	temperatureSensor.stop();
}

TEST(CurrentTemperatureImplTest, readTemperaturePeriodicallyOnFailed_TIMING) {
	const float expectedTemperature = 35.646;

	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();

	EXPECT_CALL(*mockSensorReader, getSensorName()).Times(AnyNumber());
	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
			Times(2).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Return(expectedTemperature));

	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	temperatureSensor.start(std::chrono::milliseconds(1000), std::vector<std::chrono::milliseconds>{ std::chrono::milliseconds(10) });
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	EXPECT_THAT(temperatureSensor.getCurrentTemperature(), Eq(expectedTemperature));
	temperatureSensor.stop();
}

