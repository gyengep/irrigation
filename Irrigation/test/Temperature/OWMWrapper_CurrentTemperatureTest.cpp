#include <gmock/gmock.h>
#include <fstream>
#include <stdexcept>
#include "Temperature/OWMWrapper.h"
#include "Mocks/MockNetworkReader.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(OWMHandler_CurrentTemperatureTest, parseTemperatureForecastXml) {
	ifstream input("OpenWeatherMap_current.xml");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	EXPECT_THAT(OWMWrapper::parseCurrentTemperatureXml(string(inputData.data(), inputData.size())), Eq(22.22f));
}

TEST(OWMHandler_CurrentTemperatureTest, parseXmlInvalid) {
	EXPECT_THROW(OWMWrapper::parseCurrentTemperatureXml("asdfghjkl"), runtime_error);
}

TEST(OWMHandler_CurrentTemperatureTest, parseXmlEmpty) {
	EXPECT_THROW(OWMWrapper::parseCurrentTemperatureXml(""), runtime_error);
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

	EXPECT_THAT(OWMWrapper(mockNetworkReader).readCurrentTemperature(), Eq(22.22f));
}

TEST(OWMHandler_CurrentTemperatureTest, readException) {
	const shared_ptr<MockNetworkReader> mockNetworkReader(new MockNetworkReader());

	EXPECT_CALL(*mockNetworkReader, read(_)).
		Times(AnyNumber()).
		WillRepeatedly(Throw(exception()));

	EXPECT_THROW(OWMWrapper(mockNetworkReader).readCurrentTemperature(), exception);
}
