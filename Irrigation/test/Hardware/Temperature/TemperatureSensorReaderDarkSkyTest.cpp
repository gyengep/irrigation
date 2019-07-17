#include <gmock/gmock.h>
#include <fstream>
#include "Hardware/Temperature/TemperatureSensorReaderDarkSky.h"
#include "Mocks/MockNetworkReader.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(DarkSky_TemperatureSensorReaderTest, parseJson) {
	ifstream input("DarkSky_current.json");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	EXPECT_THAT(DarkSky::TemperatureSensorReader::parseJson(string(inputData.data(), inputData.size())), Eq(26.51f));
}

TEST(DarkSky_TemperatureSensorReaderTest, parseJsonInvalid) {
	EXPECT_THROW(DarkSky::TemperatureSensorReader::parseJson("asdfghjkl"), exception);
}

TEST(DarkSky_TemperatureSensorReaderTest, parseJsonEmpty) {
	EXPECT_THROW(DarkSky::TemperatureSensorReader::parseJson("{}"), exception);
}

TEST(DarkSky_TemperatureSensorReaderTest, read) {
	ifstream input("DarkSky_current.json");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	const shared_ptr<MockNetworkReader> mockNetworkReader(new MockNetworkReader());

	EXPECT_CALL(*mockNetworkReader, read(_)).
		Times(AnyNumber()).
		WillRepeatedly(Return(string(inputData.data(), inputData.size())));

	EXPECT_THAT(DarkSky::TemperatureSensorReader(mockNetworkReader).read(), Eq(26.51f));
}

TEST(DarkSky_TemperatureSensorReaderTest, readException) {
	const shared_ptr<MockNetworkReader> mockNetworkReader(new MockNetworkReader());

	EXPECT_CALL(*mockNetworkReader, read(_)).
		Times(AnyNumber()).
		WillRepeatedly(Throw(exception()));

	EXPECT_THROW(DarkSky::TemperatureSensorReader(mockNetworkReader).read(), exception);
}
