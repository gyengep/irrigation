#include <gmock/gmock.h>
#include "Mocks/MockFileReader.h"
#include "Mocks/MockTemperatureSensor.h"
#include "Hardware/Temperature/Temperature.h"
#include "Exceptions/Exceptions.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////


TEST(TemperatureTest, isValid) {
	Temperature temp(make_shared<MockTemperatureSensor>());
	EXPECT_FALSE(temp.isValid());
}

TEST(TemperatureTest, getCachedValue) {
	const float expectedTemperature = 35.646;
	const shared_ptr<MockTemperatureSensor> mockTemperatureSensor = make_shared<MockTemperatureSensor>();

	EXPECT_CALL(*mockTemperatureSensor, readValueFromSensor()).
		Times(AnyNumber()).
		WillRepeatedly(Return(expectedTemperature));

	Temperature temp(mockTemperatureSensor);
	temp.refresh();

	EXPECT_TRUE(temp.isValid());
	EXPECT_THAT(temp.getCachedValue(), Eq(expectedTemperature));
}

TEST(TemperatureTest, getCachedValue_invalidFile) {
	const shared_ptr<MockTemperatureSensor> mockTemperatureSensor = make_shared<MockTemperatureSensor>();

	EXPECT_CALL(*mockTemperatureSensor, readValueFromSensor()).
		Times(AnyNumber()).
		WillRepeatedly(Throw(TemperatureException("")));

	Temperature temp(mockTemperatureSensor);
	temp.refresh();

	EXPECT_FALSE(temp.isValid());
	EXPECT_THROW(temp.getCachedValue(), logic_error);
}
