#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureSensorImpl.h"
#include "Hardware/Temperature/TemperatureException.h"
#include "Mocks/MockTemperatureSensorReader.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureSensorImplTest, getCachedValue) {
	const float expectedTemperature = 35.646;

	auto mockSensorReader = make_shared<MockTemperatureSensorReader>();
	TemperatureSensorImpl temperatureSensor(mockSensorReader);

	EXPECT_CALL(*mockSensorReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(expectedTemperature));

	temperatureSensor.updateCache();

	EXPECT_THAT(temperatureSensor.getCachedValue(), Eq(expectedTemperature));
}

TEST(TemperatureSensorImplTest, getCachedValue_invalid) {
	auto mockSensorReader = make_shared<MockTemperatureSensorReader>();
	TemperatureSensorImpl temperatureSensor(mockSensorReader);

	EXPECT_CALL(*mockSensorReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Throw(TemperatureException("")));

	temperatureSensor.updateCache();

	EXPECT_THROW(temperatureSensor.getCachedValue(), TemperatureException);
}

TEST(TemperatureSensorImplTest, onTimer) {
	auto mockSensorReader = make_shared<MockTemperatureSensorReader>();

	EXPECT_CALL(*mockSensorReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(25));

	TemperatureSensorImpl temperatureSensor(mockSensorReader);

	EXPECT_NO_THROW(temperatureSensor.onTimer());
}
