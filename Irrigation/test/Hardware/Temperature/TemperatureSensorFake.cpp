#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureSensorFake.h"
#include "Exceptions/Exceptions.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureSensorFakeTest, readValueFromSensor) {
	EXPECT_THROW(TemperatureSensor_Fake().readValueFromSensor(), runtime_error);
}
