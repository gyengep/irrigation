#include <gmock/gmock.h>
#include <fstream>
#include <stdexcept>
#include "Mocks/MockNetworkReader.h"
#include "Temperature/OWMWrapper.h"
#include "Utils/TimeConversion.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(OWMHandler_TemperatureForecastTest, validateTimeString) {
	EXPECT_NO_THROW(OWMWrapper::parseTimeString("2019-06-18T15:53:00"));
	EXPECT_ANY_THROW(OWMWrapper::parseTimeString("2A19-06-17T18:00:00"));
	EXPECT_ANY_THROW(OWMWrapper::parseTimeString("219-06-17T18:00:00"));
	EXPECT_ANY_THROW(OWMWrapper::parseTimeString("A1c"));
	EXPECT_ANY_THROW(OWMWrapper::parseTimeString("1"));
	EXPECT_ANY_THROW(OWMWrapper::parseTimeString("456"));
	EXPECT_ANY_THROW(OWMWrapper::parseTimeString("ABC"));
	EXPECT_ANY_THROW(OWMWrapper::parseTimeString("abcd"));
}

TEST(OWMHandler_TemperatureForecastTest, parseTimeString) {
	EXPECT_THAT(OWMWrapper::parseTimeString("2019-06-18T15:54:08"), Eq(1560873248));
	EXPECT_THAT(OWMWrapper::parseTimeString("2019-01-10T13:14:15"), Eq(1547126055));
}

TEST(OWMHandler_TemperatureForecastTest, parseTemperatureForecastXml) {
	ifstream input("OpenWeatherMap_forecast.xml");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	EXPECT_THAT(fromUtcTime(2019, 6, 27, 18, 0, 0), Eq(1561658400));
	EXPECT_THAT(fromUtcTime(2019, 6, 27, 21, 0, 0), Eq(1561669200));
	EXPECT_THAT(fromUtcTime(2019, 6, 28, 0, 0, 0), Eq(1561680000));

	EXPECT_THAT(OWMWrapper::parseTemperatureForecastXml(string(inputData.data(), inputData.size())), ElementsAreArray({
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 27, 18, 0, 0), fromUtcTime(2019, 6, 27, 21, 0, 0), 18.25, 19.85),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 27, 21, 0, 0), fromUtcTime(2019, 6, 28, 0, 0, 0), 15.54, 16.74),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 28, 0, 0, 0), fromUtcTime(2019, 6, 28, 3, 0, 0), 13.47, 14.27),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 28, 3, 0, 0), fromUtcTime(2019, 6, 28, 6, 0, 0), 17.45, 17.85),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 28, 6, 0, 0), fromUtcTime(2019, 6, 28, 9, 0, 0), 20.95, 20.95),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 28, 9, 0, 0), fromUtcTime(2019, 6, 28, 12, 0, 0), 23.35, 23.35),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 28, 12, 0, 0), fromUtcTime(2019, 6, 28, 15, 0, 0), 22.67, 22.67),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 28, 15, 0, 0), fromUtcTime(2019, 6, 28, 18, 0, 0), 19, 19),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 28, 18, 0, 0), fromUtcTime(2019, 6, 28, 21, 0, 0), 15.04, 15.04),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 28, 21, 0, 0), fromUtcTime(2019, 6, 29, 0, 0, 0), 13.87, 13.87),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 29, 0, 0, 0), fromUtcTime(2019, 6, 29, 3, 0, 0), 13.06, 13.06),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 29, 3, 0, 0), fromUtcTime(2019, 6, 29, 6, 0, 0), 17.98, 17.98),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 29, 6, 0, 0), fromUtcTime(2019, 6, 29, 9, 0, 0), 21.75, 21.75),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 29, 9, 0, 0), fromUtcTime(2019, 6, 29, 12, 0, 0), 23.83, 23.83),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 29, 12, 0, 0), fromUtcTime(2019, 6, 29, 15, 0, 0), 24.16, 24.16),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 29, 15, 0, 0), fromUtcTime(2019, 6, 29, 18, 0, 0), 20.83, 20.83),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 29, 18, 0, 0), fromUtcTime(2019, 6, 29, 21, 0, 0), 16.51, 16.51),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 29, 21, 0, 0), fromUtcTime(2019, 6, 30, 0, 0, 0), 14.57, 14.57),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 30, 0, 0, 0), fromUtcTime(2019, 6, 30, 3, 0, 0), 13.25, 13.25),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 30, 3, 0, 0), fromUtcTime(2019, 6, 30, 6, 0, 0), 18.15, 18.15),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 30, 6, 0, 0), fromUtcTime(2019, 6, 30, 9, 0, 0), 23.65, 23.65),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 30, 9, 0, 0), fromUtcTime(2019, 6, 30, 12, 0, 0), 26.35, 26.35),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 30, 12, 0, 0), fromUtcTime(2019, 6, 30, 15, 0, 0), 26.35, 26.35),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 30, 15, 0, 0), fromUtcTime(2019, 6, 30, 18, 0, 0), 22.18, 22.18),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 30, 18, 0, 0), fromUtcTime(2019, 6, 30, 21, 0, 0), 18.75, 18.75),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 30, 21, 0, 0), fromUtcTime(2019, 7, 1, 0, 0, 0), 17.25, 17.25),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 1, 0, 0, 0), fromUtcTime(2019, 7, 1, 3, 0, 0), 16.67, 16.67),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 1, 3, 0, 0), fromUtcTime(2019, 7, 1, 6, 0, 0), 20.75, 20.75),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 1, 6, 0, 0), fromUtcTime(2019, 7, 1, 9, 0, 0), 25.44, 25.44),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 1, 9, 0, 0), fromUtcTime(2019, 7, 1, 12, 0, 0), 28.15, 28.15),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 1, 12, 0, 0), fromUtcTime(2019, 7, 1, 15, 0, 0), 27.86, 27.86),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 1, 15, 0, 0), fromUtcTime(2019, 7, 1, 18, 0, 0), 23.24, 23.24),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 1, 18, 0, 0), fromUtcTime(2019, 7, 1, 21, 0, 0), 20.45, 20.45),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 1, 21, 0, 0), fromUtcTime(2019, 7, 2, 0, 0, 0), 19.05, 19.05),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 2, 0, 0, 0), fromUtcTime(2019, 7, 2, 3, 0, 0), 17.85, 17.85),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 2, 3, 0, 0), fromUtcTime(2019, 7, 2, 6, 0, 0), 22.14, 22.14),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 2, 6, 0, 0), fromUtcTime(2019, 7, 2, 9, 0, 0), 25.35, 25.35),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 2, 9, 0, 0), fromUtcTime(2019, 7, 2, 12, 0, 0), 26.24, 26.24),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 2, 12, 0, 0), fromUtcTime(2019, 7, 2, 15, 0, 0), 24.93, 24.93),
			TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 7, 2, 15, 0, 0), fromUtcTime(2019, 7, 2, 18, 0, 0), 20.75, 20.75)
	}));
}

TEST(OWMHandler_TemperatureForecastTest, parseXmlInvalid) {
	EXPECT_ANY_THROW(OWMWrapper::parseTemperatureForecastXml("abc"));
}

TEST(OWMHandler_TemperatureForecastTest, parseXmlEmpty) {
	EXPECT_THAT(OWMWrapper::parseTemperatureForecastXml("<?xml version=\"1.0\" encoding=\"UTF-8\"?><weatherdata></weatherdata>"), IsEmpty());
}

TEST(OWMHandler_TemperatureForecastTest, readTemperatureForecast) {

	const string xml =
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
	"<weatherdata>"
	    "<forecast>"
	        "<time from=\"2019-06-27T18:00:00\" to=\"2019-06-27T21:00:00\">"
	            "<temperature unit=\"celsius\" value=\"19.85\" min=\"18.25\" max=\"19.85\"></temperature>"
	        "</time>"
	        "<time from=\"2019-06-27T21:00:00\" to=\"2019-06-28T00:00:00\">"
	            "<temperature unit=\"celsius\" value=\"16.74\" min=\"15.54\" max=\"16.74\"></temperature>"
	        "</time>"
	        "<time from=\"2019-06-28T00:00:00\" to=\"2019-06-28T03:00:00\">"
	            "<temperature unit=\"celsius\" value=\"14.27\" min=\"13.47\" max=\"14.27\"></temperature>"
	        "</time>"
	    "</forecast>"
	"</weatherdata>";

	const list<TemperatureForecastProvider::ValuesWithTimes> expectedList {
		TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 27, 18, 0, 0), fromUtcTime(2019, 6, 27, 21, 0, 0), 18.25, 19.85),
		TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 27, 21, 0, 0), fromUtcTime(2019, 6, 28, 0, 0, 0), 15.54, 16.74),
		TemperatureForecastProvider::ValuesWithTimes(fromUtcTime(2019, 6, 28, 0, 0, 0), fromUtcTime(2019, 6, 28, 3, 0, 0), 13.47, 14.27)
	};

	auto mockTemperatureForecastReader = make_shared<MockNetworkReader>();
	EXPECT_CALL(*mockTemperatureForecastReader, read(_)).Times(1).WillOnce(Return(xml));

	OWMWrapper temperatureForecast(mockTemperatureForecastReader);
	EXPECT_THAT(temperatureForecast.readTemperatureForecast(), ContainerEq(expectedList));
}

TEST(OWMHandler_TemperatureForecastTest, getForecastException) {

	auto mockTemperatureForecastReader = make_shared<MockNetworkReader>();
	EXPECT_CALL(*mockTemperatureForecastReader, read(_)).Times(1).WillOnce(Throw(exception()));

	OWMWrapper temperatureForecast(mockTemperatureForecastReader);
	EXPECT_THROW(temperatureForecast.readTemperatureForecast(), exception);
}
