#include <gmock/gmock.h>
#include <fstream>
#include "Hardware/Temperature/TemperatureSensorReaderOWM.h"
#include "Mocks/MockNetworkReader.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(OpenWeatherMap_TemperatureSensorReaderTest, parseXml) {
	ifstream input("OpenWeatherMap_current.xml");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	EXPECT_THAT(OpenWeatherMap::TemperatureSensorReader::parseXml(string(inputData.data(), inputData.size())), Eq(22.22f));
}

TEST(OpenWeatherMap_TemperatureSensorReaderTest, parseXmlInvalid) {
	EXPECT_THROW(OpenWeatherMap::TemperatureSensorReader::parseXml("asdfghjkl"), runtime_error);
}

TEST(OpenWeatherMap_TemperatureSensorReaderTest, parseXmlEmpty) {
	EXPECT_THROW(OpenWeatherMap::TemperatureSensorReader::parseXml(""), runtime_error);
}

TEST(OpenWeatherMap_TemperatureSensorReaderTest, read) {
	ifstream input("OpenWeatherMap_current.xml");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	const shared_ptr<MockNetworkReader> mockNetworkReader(new MockNetworkReader());

	EXPECT_CALL(*mockNetworkReader, read(_)).
		Times(AnyNumber()).
		WillRepeatedly(Return(string(inputData.data(), inputData.size())));

	EXPECT_THAT(OpenWeatherMap::TemperatureSensorReader(mockNetworkReader).read(), Eq(22.22f));
}

TEST(OpenWeatherMap_TemperatureSensorReaderTest, readException) {
	const shared_ptr<MockNetworkReader> mockNetworkReader(new MockNetworkReader());

	EXPECT_CALL(*mockNetworkReader, read(_)).
		Times(AnyNumber()).
		WillRepeatedly(Throw(exception()));

	EXPECT_THROW(OpenWeatherMap::TemperatureSensorReader(mockNetworkReader).read(), exception);
}
