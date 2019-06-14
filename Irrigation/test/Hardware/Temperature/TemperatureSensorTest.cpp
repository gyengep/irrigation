#include <gmock/gmock.h>
#include "Hardware/Temperature/Temperature.h"
#include "Mocks/MockTemperatureSensor.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureSensorTest, getCachedValue) {
	const float expectedTemperature = 35.646;

	MockTemperatureSensor tempSensor;

	EXPECT_CALL(tempSensor, readValueFromSensor()).
		Times(AnyNumber()).
		WillRepeatedly(Return(expectedTemperature));

	tempSensor.updateCache();

	EXPECT_THAT(tempSensor.getCachedValue(), Eq(expectedTemperature));
}

TEST(TemperatureSensorTest, getCachedValue_invalid) {
	MockTemperatureSensor tempSensor;

	EXPECT_CALL(tempSensor, readValueFromSensor()).
		Times(AnyNumber()).
		WillRepeatedly(Throw(TemperatureException("")));

	tempSensor.updateCache();

	EXPECT_THROW(tempSensor.getCachedValue(), TemperatureException);
}
