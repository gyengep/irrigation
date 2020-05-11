#include <gmock/gmock.h>
#include "Hardware/DS18B20/DS18B20TemperatureSensor.h"
#include "Mocks/MockFileReader.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

static const string content =
		"f2 00 4b 46 7f ff 0c 10 69 : crc=69 YES\n"
		"f2 00 4b 46 7f ff 0c 10 69 t=15125\n";


TEST(DS18B20TemperatureSensorTest, parseText) {
	EXPECT_THAT(DS18B20TemperatureSensor::parseText(content), Eq(15.125f));
}

TEST(DS18B20TemperatureSensorTest, parseTextInvalid) {
	EXPECT_THROW(DS18B20TemperatureSensor::parseText("asdfghjkl"), runtime_error);
}

TEST(DS18B20TemperatureSensorTest, parseTextEmpty) {
	EXPECT_THROW(DS18B20TemperatureSensor::parseText(""), runtime_error);
}

TEST(DS18B20TemperatureSensorTest, read) {
	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(content));

	EXPECT_THAT(DS18B20TemperatureSensor(mockFileReader).read(), Eq(15.125f));
}

TEST(DS18B20TemperatureSensorTest, readException) {
	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Throw(exception()));

	EXPECT_THROW(DS18B20TemperatureSensor(mockFileReader).read(), exception);
}
