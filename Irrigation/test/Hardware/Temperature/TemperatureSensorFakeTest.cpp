#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureSensorFake.h"
#include "Hardware/Temperature/TemperatureException.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureSensorFakeTest, readValueFromSensor) {
	EXPECT_THROW(TemperatureSensor_Fake().readValueFromSensor(), TemperatureException);
}
