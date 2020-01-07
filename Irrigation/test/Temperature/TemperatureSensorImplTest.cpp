#include <gmock/gmock.h>
#include "Temperature/CurrentTemperatureImpl.h"
#include "Temperature/TemperatureException.h"
#include "Mocks/MockCurrentTemperatureProvider.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(CurrentTemperatureImplTest, getCurrentTemperature) {
	const float expectedTemperature = 35.646;

	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();
	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
		Times(AnyNumber()).
		WillRepeatedly(Return(expectedTemperature));

	temperatureSensor.updateCache();

	EXPECT_THAT(temperatureSensor.getCurrentTemperature(), Eq(expectedTemperature));
}

TEST(CurrentTemperatureImplTest, getCurrentTemperature_invalid) {
	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();
	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
		Times(AnyNumber()).
		WillRepeatedly(Throw(exception()));

	temperatureSensor.updateCache();

	EXPECT_THROW(temperatureSensor.getCurrentTemperature(), TemperatureException);
}

TEST(CurrentTemperatureImplTest, onTimer) {
	auto mockSensorReader = make_shared<MockCurrentTemperatureProvider>();

	EXPECT_CALL(*mockSensorReader, readCurrentTemperature()).
		Times(AnyNumber()).
		WillRepeatedly(Return(25));

	CurrentTemperatureImpl temperatureSensor(mockSensorReader);

	EXPECT_NO_THROW(temperatureSensor.onTimer());
}
