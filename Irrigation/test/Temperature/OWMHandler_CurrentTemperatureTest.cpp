#include <gmock/gmock.h>
#include <fstream>
#include <stdexcept>
#include "Temperature/OWMHandler.h"
#include "Mocks/MockNetworkReader.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(OWMHandler_CurrentTemperatureTest, parseTemperatureForecastXml) {
	ifstream input("OpenWeatherMap_current.xml");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	EXPECT_THAT(OWMHandler::parseCurrentTemperatureXml(string(inputData.data(), inputData.size())), Eq(22.22f));
}

TEST(OWMHandler_CurrentTemperatureTest, parseXmlInvalid) {
	EXPECT_THROW(OWMHandler::parseCurrentTemperatureXml("asdfghjkl"), runtime_error);
}

TEST(OWMHandler_CurrentTemperatureTest, parseXmlEmpty) {
	EXPECT_THROW(OWMHandler::parseCurrentTemperatureXml(""), runtime_error);
}

TEST(OWMHandler_CurrentTemperatureTest, read) {
	ifstream input("OpenWeatherMap_current.xml");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	const shared_ptr<MockNetworkReader> mockNetworkReader(new MockNetworkReader());

	EXPECT_CALL(*mockNetworkReader, read(_)).
		Times(AnyNumber()).
		WillRepeatedly(Return(string(inputData.data(), inputData.size())));

	EXPECT_THAT(OWMHandler(mockNetworkReader).readCurrentTemperature(), Eq(22.22f));
}

TEST(OWMHandler_CurrentTemperatureTest, readException) {
	const shared_ptr<MockNetworkReader> mockNetworkReader(new MockNetworkReader());

	EXPECT_CALL(*mockNetworkReader, read(_)).
		Times(AnyNumber()).
		WillRepeatedly(Throw(exception()));

	EXPECT_THROW(OWMHandler(mockNetworkReader).readCurrentTemperature(), exception);
}
