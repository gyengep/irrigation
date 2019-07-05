#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureSensor.h"
#include "Hardware/Temperature/TemperatureException.h"
#include "Mocks/MockTemperatureSensorReader.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureSensorTest, getCachedValue) {
	const float expectedTemperature = 35.646;

	auto mockSensorReader = make_shared<MockTemperatureSensorReader>();
	TemperatureSensor temperatureSensor(mockSensorReader);

	EXPECT_CALL(*mockSensorReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(expectedTemperature));

	temperatureSensor.updateCache();

	EXPECT_THAT(temperatureSensor.getCachedValue(), Eq(expectedTemperature));
}

TEST(TemperatureSensorTest, getCachedValue_invalid) {
	auto mockSensorReader = make_shared<MockTemperatureSensorReader>();
	TemperatureSensor temperatureSensor(mockSensorReader);

	EXPECT_CALL(*mockSensorReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Throw(TemperatureException("")));

	temperatureSensor.updateCache();

	EXPECT_THROW(temperatureSensor.getCachedValue(), TemperatureException);
}
