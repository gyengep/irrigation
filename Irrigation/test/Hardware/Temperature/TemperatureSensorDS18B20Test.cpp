#include <gmock/gmock.h>
#include "Mocks/MockFileReader.h"
#include "Hardware/Temperature/TemperatureSensorDS18B20.h"
#include "Hardware/Temperature/TemperatureException.h"
#include "Exceptions/Exceptions.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureSensorDS18B20Test, readValueFromSensor) {
	const string content =
			"f2 00 4b 46 7f ff 0c 10 69 : crc=69 YES\n"
			"f2 00 4b 46 7f ff 0c 10 69 t=15125\n";

	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(content));

	EXPECT_THAT(TemperatureSensor_DS18B20(mockFileReader).readValueFromSensor(), Eq(15.125f));
}

TEST(TemperatureSensorDS18B20Test, readValueFromSensor_invalidContent) {
	const string content = "asdfghjkl";
	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(content));

	EXPECT_THROW(TemperatureSensor_DS18B20(mockFileReader).readValueFromSensor(), TemperatureException);
}

TEST(TemperatureSensorDS18B20Test, readValueFromSensor_invalidFile) {
	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Throw(FileNotFoundException()));

	EXPECT_THROW(TemperatureSensor_DS18B20(mockFileReader).readValueFromSensor(), FileNotFoundException);
}

