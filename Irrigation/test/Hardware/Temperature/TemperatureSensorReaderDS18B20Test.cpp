#include <gmock/gmock.h>
#include "Hardware/Temperature/TemperatureSensorReaderDS18B20.h"
#include "Mocks/MockFileReader.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

static const string content =
		"f2 00 4b 46 7f ff 0c 10 69 : crc=69 YES\n"
		"f2 00 4b 46 7f ff 0c 10 69 t=15125\n";


TEST(DS18B20_TemperatureSensorReaderTest, parseText) {
	EXPECT_THAT(DS18B20::TemperatureSensorReader::parseText(content), Eq(15.125f));
}

TEST(DS18B20_TemperatureSensorReaderTest, parseTextInvalid) {
	EXPECT_THROW(DS18B20::TemperatureSensorReader::parseText("asdfghjkl"), runtime_error);
}

TEST(DS18B20_TemperatureSensorReaderTest, parseTextEmpty) {
	EXPECT_THROW(DS18B20::TemperatureSensorReader::parseText(""), runtime_error);
}

TEST(DS18B20_TemperatureSensorReaderTest, read) {
	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(content));

	EXPECT_THAT(DS18B20::TemperatureSensorReader(mockFileReader).read(), Eq(15.125f));
}

TEST(DS18B20_TemperatureSensorReaderTest, readException) {
	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Throw(exception()));

	EXPECT_THROW(DS18B20::TemperatureSensorReader(mockFileReader).read(), exception);
}
