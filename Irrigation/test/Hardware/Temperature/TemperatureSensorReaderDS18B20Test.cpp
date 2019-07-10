#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Hardware/Temperature/TemperatureSensorReaderDS18B20.h"
#include "Hardware/Temperature/TemperatureException.h"
#include "Mocks/MockFileReader.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureSensorReaderDS18B20Test, read) {
	const string content =
			"f2 00 4b 46 7f ff 0c 10 69 : crc=69 YES\n"
			"f2 00 4b 46 7f ff 0c 10 69 t=15125\n";

	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(content));

	EXPECT_THAT(TemperatureSensorReader_DS18B20(mockFileReader).read(), Eq(15.125f));
}

TEST(TemperatureSensorReaderDS18B20Test, readValueFromSensor_invalidContent) {
	const string content = "asdfghjkl";
	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(content));

	EXPECT_THROW(TemperatureSensorReader_DS18B20(mockFileReader).read(), TemperatureException);
}

TEST(TemperatureSensorReaderDS18B20Test, readValueFromSensor_invalidFile) {
	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Throw(FileNotFoundException()));

	EXPECT_THROW(TemperatureSensorReader_DS18B20(mockFileReader).read(), FileNotFoundException);
}

