#include <gmock/gmock.h>
#include "Mocks/MockFileReader.h"
#include "Hardware/Temperature/Temperature.h"
#include "Exceptions/Exceptions.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureTest, readValueFromSensor) {
	const string content =
			"f2 00 4b 46 7f ff 0c 10 69 : crc=69 YES\n"
			"f2 00 4b 46 7f ff 0c 10 69 t=15125\n";

	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(content));

	Temperature temp(mockFileReader);

	EXPECT_THAT(temp.readValueFromSensor(), Eq(15.125f));
}

TEST(TemperatureTest, readValueFromSensor_invalidContent) {
	const string content = "asdfghjkl";

	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(content));

	Temperature temp(mockFileReader);

	EXPECT_THROW(temp.readValueFromSensor(), TemperatureException);
}

TEST(TemperatureTest, readValueFromSensor_invalidFile) {
	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Throw(FileNotFoundException()));

	Temperature temp(mockFileReader);

	EXPECT_THROW(temp.readValueFromSensor(), FileNotFoundException);
}


TEST(TemperatureTest, isValid) {
	Temperature temp(make_shared<MockFileReader>());
	EXPECT_FALSE(temp.isValid());
}

TEST(TemperatureTest, getCachedValue) {
	const string content =
			"f2 00 4b 46 7f ff 0c 10 69 : crc=69 YES\n"
			"f2 00 4b 46 7f ff 0c 10 69 t=15125\n";

	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(content));

	Temperature temp(mockFileReader);

	temp.refresh();

	EXPECT_TRUE(temp.isValid());
	EXPECT_THAT(temp.getCachedValue(), Eq(15.125f));
}

TEST(TemperatureTest, getCachedValue_invalidContent) {
	const string content = "asdfghjkl";

	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Return(content));

	Temperature temp(mockFileReader);

	temp.refresh();

	EXPECT_FALSE(temp.isValid());
	EXPECT_THROW(temp.getCachedValue(), logic_error);
}

TEST(TemperatureTest, getCachedValue_invalidFile) {
	const shared_ptr<MockFileReader> mockFileReader(new MockFileReader());

	EXPECT_CALL(*mockFileReader, read()).
		Times(AnyNumber()).
		WillRepeatedly(Throw(FileNotFoundException()));

	Temperature temp(mockFileReader);

	temp.refresh();

	EXPECT_FALSE(temp.isValid());
	EXPECT_THROW(temp.getCachedValue(), logic_error);
}
