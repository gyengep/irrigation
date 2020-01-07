#include <gmock/gmock.h>
#include <fstream>
#include "Temperature/DarkSkyHandler.h"
#include "Mocks/MockNetworkReader.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(DarkSkyHandler_CurrentTemperatureTest, parseJson) {
	ifstream input("DarkSky_current.json");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	EXPECT_THAT(DarkSkyHandler::parseCurrentTemperatureJson(string(inputData.data(), inputData.size())), Eq(26.51f));
}

TEST(DarkSkyHandler_CurrentTemperatureTest, parseJsonInvalid) {
	EXPECT_THROW(DarkSkyHandler::parseCurrentTemperatureJson("asdfghjkl"), exception);
}

TEST(DarkSkyHandler_CurrentTemperatureTest, parseJsonEmpty) {
	EXPECT_THROW(DarkSkyHandler::parseCurrentTemperatureJson("{}"), exception);
}

TEST(DarkSkyHandler_CurrentTemperatureTest, readCurrentTemperature) {
	ifstream input("DarkSky_current.json");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	const shared_ptr<MockNetworkReader> mockNetworkReader(new MockNetworkReader());

	EXPECT_CALL(*mockNetworkReader, read(_)).
		Times(AnyNumber()).
		WillRepeatedly(Return(string(inputData.data(), inputData.size())));

	EXPECT_THAT(DarkSkyHandler(mockNetworkReader).readCurrentTemperature(), Eq(26.51f));
}

TEST(DarkSkyHandler_CurrentTemperatureTest, readException) {
	const shared_ptr<MockNetworkReader> mockNetworkReader(new MockNetworkReader());

	EXPECT_CALL(*mockNetworkReader, read(_)).
		Times(AnyNumber()).
		WillRepeatedly(Throw(exception()));

	EXPECT_THROW(DarkSkyHandler(mockNetworkReader).readCurrentTemperature(), exception);
}
