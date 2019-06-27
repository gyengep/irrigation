#include <gmock/gmock.h>
#include <fstream>
#include "Hardware/Temperature/TemperatureForecast.h"
#include "Utils/TimeConversion.h"


using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

class MockTemperatureForecastReader : public TemperatureForecast::NetworkReader {
public:
	MOCK_CONST_METHOD3(read, string(const string& url, const string& location, const string& appid));
};

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureForecastTest, validateTimeString) {
	EXPECT_NO_THROW(TemperatureForecast::parseTimeString("2019-06-18T15:53:00"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("2A19-06-17T18:00:00"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("219-06-17T18:00:00"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("A1c"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("1"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("456"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("ABC"));
	EXPECT_ANY_THROW(TemperatureForecast::parseTimeString("abcd"));
}

TEST(TemperatureForecastTest, parseTimeString) {
	EXPECT_THAT(TemperatureForecast::parseTimeString("2019-06-18T15:54:08"), Eq(1560873248));
	EXPECT_THAT(TemperatureForecast::parseTimeString("2019-01-10T13:14:15"), Eq(1547126055));
}

TEST(TemperatureForecastTest, addTemperature) {
	TemperatureForecast temperatureForecast;

	temperatureForecast.addTemperature(10, 20, TemperatureForecast::Values(12, 14));
	temperatureForecast.addTemperature(20, 30, TemperatureForecast::Values(16, 18));
	temperatureForecast.addTemperature(30, 40, TemperatureForecast::Values(22, 28));

	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(list<TemperatureForecast::ValuesWithTimes>{
		TemperatureForecast::ValuesWithTimes(10, 20, TemperatureForecast::Values(12, 14)),
		TemperatureForecast::ValuesWithTimes(20, 30, TemperatureForecast::Values(16, 18)),
		TemperatureForecast::ValuesWithTimes(30, 40, TemperatureForecast::Values(22, 28))
	}));
}

TEST(TemperatureForecastTest, addTemperatureInvalid) {
	TemperatureForecast temperatureForecast;

	EXPECT_ANY_THROW(temperatureForecast.addTemperature(30, 30, TemperatureForecast::Values(12, 14)));
	EXPECT_ANY_THROW(temperatureForecast.addTemperature(20, 10, TemperatureForecast::Values(12, 14)));
}

TEST(TemperatureForecastTest, addTemperatureMismatch) {
	TemperatureForecast temperatureForecast;

	EXPECT_NO_THROW(temperatureForecast.addTemperature(10, 20, TemperatureForecast::Values(12, 14)));
	EXPECT_ANY_THROW(temperatureForecast.addTemperature(19, 30, TemperatureForecast::Values(12, 14)));
	EXPECT_ANY_THROW(temperatureForecast.addTemperature(21, 30, TemperatureForecast::Values(12, 14)));
}

TEST(TemperatureForecastTest, getForecastValues) {
	TemperatureForecast temperatureForecast;

	temperatureForecast.addTemperature(10, 20, TemperatureForecast::Values(12, 14));
	temperatureForecast.addTemperature(20, 30, TemperatureForecast::Values(16, 18));
	temperatureForecast.addTemperature(30, 40, TemperatureForecast::Values(20, 22));
	temperatureForecast.addTemperature(40, 50, TemperatureForecast::Values(24, 26));
	temperatureForecast.addTemperature(50, 60, TemperatureForecast::Values(28, 30));

	EXPECT_THAT(temperatureForecast.getForecastValues(20, 40), TemperatureForecast::Values(16, 22));
	EXPECT_THAT(temperatureForecast.getForecastValues(15, 40), TemperatureForecast::Values(12, 22));
	EXPECT_THAT(temperatureForecast.getForecastValues(15, 45), TemperatureForecast::Values(12, 26));
	EXPECT_THAT(temperatureForecast.getForecastValues(20, 45), TemperatureForecast::Values(16, 26));
}

TEST(TemperatureForecastTest, getForecastOutOfBounds) {
	TemperatureForecast temperatureForecast;

	temperatureForecast.addTemperature(10, 20, TemperatureForecast::Values(12, 14));
	temperatureForecast.addTemperature(20, 30, TemperatureForecast::Values(16, 18));
	temperatureForecast.addTemperature(30, 40, TemperatureForecast::Values(20, 22));
	temperatureForecast.addTemperature(40, 50, TemperatureForecast::Values(24, 26));
	temperatureForecast.addTemperature(50, 60, TemperatureForecast::Values(28, 30));

	EXPECT_ANY_THROW(temperatureForecast.getForecastValues(60, 70));
	EXPECT_ANY_THROW(temperatureForecast.getForecastValues(5, 10));
}

const string expectedXml1 =
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		"<weatherdata>"
			"<forecast>"
				"<time from= \"2019-06-20T18:00:00\" to=\"2019-06-20T21:00:00\">"
					"<temperature unit=\"celsius\" value=\"17.84\" min=\"17.84\" max=\"18.6\"></temperature>"
				"</time>"
				"<time from=\"2019-06-20T21:00:00\" to=\"2019-06-21T00:00:00\">"
					"<temperature unit=\"celsius\" value=\"16.86\" min=\"16.86\" max=\"17.43\"></temperature>"
				"</time>"
			"</forecast>"
		"</weatherdata>";


const list<TemperatureForecast::ValuesWithTimes> expectedList1 {
	TemperatureForecast::ValuesWithTimes(1561053600, 1561064400, TemperatureForecast::Values(17.84, 18.6)),
	TemperatureForecast::ValuesWithTimes(1561064400, 1561075200, TemperatureForecast::Values(16.86, 17.43))
};


TEST(TemperatureForecastTest, parseXml) {
	EXPECT_THAT(TemperatureForecast::parseXml(expectedXml1), ContainerEq(expectedList1));
}

TEST(TemperatureForecastTest, parseXmlInvalid) {
	EXPECT_ANY_THROW(TemperatureForecast::parseXml("abc"));
}

TEST(TemperatureForecastTest, parseXmlEmpty) {
	EXPECT_THAT(TemperatureForecast::parseXml("<?xml version=\"1.0\" encoding=\"UTF-8\"?><weatherdata></weatherdata>"), IsEmpty());
}

TEST(TemperatureForecastTest, updateCache) {

	auto mockTemperatureForecastReader = make_shared<MockTemperatureForecastReader>();

	EXPECT_CALL(*mockTemperatureForecastReader, read(_, _, _)).Times(1).WillOnce(Return(expectedXml1));

	TemperatureForecast temperatureForecast(mockTemperatureForecastReader);
	temperatureForecast.updateCache();

	EXPECT_THAT(temperatureForecast.getContainer(), ContainerEq(expectedList1));
}

TEST(TemperatureForecastTest, updateCacheEmpty) {
	auto mockTemperatureForecastReader = make_shared<MockTemperatureForecastReader>();

	EXPECT_CALL(*mockTemperatureForecastReader, read(_, _, _)).
			Times(1).
			WillOnce(Return("<?xml version=\"1.0\" encoding=\"UTF-8\"?><weatherdata></weatherdata>"));

	TemperatureForecast temperatureForecast(mockTemperatureForecastReader);
	temperatureForecast.updateCache();

	EXPECT_THAT(temperatureForecast.getContainer(), IsEmpty());
}

TEST(TemperatureForecastTest, updateCacheInvalid) {
	auto mockTemperatureForecastReader = make_shared<MockTemperatureForecastReader>();

	EXPECT_CALL(*mockTemperatureForecastReader, read(_, _, _)).Times(1).WillOnce(Throw(exception()));

	TemperatureForecast temperatureForecast(mockTemperatureForecastReader);
	temperatureForecast.updateCache();

	EXPECT_THAT(temperatureForecast.getContainer(), IsEmpty());
}

time_t toTime(int year, int month, int day, int hour, int min, int sec) {
	auto timeinfo = toCalendarTime(year, month, day, hour, min, sec);
	return timegm(&timeinfo);
}

TEST(TemperatureForecastTest, parseXmlFromRealXml) {
	ifstream input("temperature_forecast.xml");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	EXPECT_THAT(toTime(2019, 6, 27, 18, 0, 0), Eq(1561658400));
	EXPECT_THAT(toTime(2019, 6, 27, 21, 0, 0), Eq(1561669200));
	EXPECT_THAT(toTime(2019, 6, 28, 0, 0, 0), Eq(1561680000));

	EXPECT_THAT(TemperatureForecast::parseXml(inputData.data()), ElementsAreArray({
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 27, 21, 0, 0), TemperatureForecast::Values(18.25, 19.85)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 27, 21, 0, 0), toTime(2019, 6, 28, 0, 0, 0), TemperatureForecast::Values(15.54, 16.74)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 28, 0, 0, 0), toTime(2019, 6, 28, 3, 0, 0), TemperatureForecast::Values(13.47, 14.27)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 28, 3, 0, 0), toTime(2019, 6, 28, 6, 0, 0), TemperatureForecast::Values(17.45, 17.85)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 28, 6, 0, 0), toTime(2019, 6, 28, 9, 0, 0), TemperatureForecast::Values(20.95, 20.95)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 28, 9, 0, 0), toTime(2019, 6, 28, 12, 0, 0), TemperatureForecast::Values(23.35, 23.35)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 28, 12, 0, 0), toTime(2019, 6, 28, 15, 0, 0), TemperatureForecast::Values(22.67, 22.67)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 28, 15, 0, 0), toTime(2019, 6, 28, 18, 0, 0), TemperatureForecast::Values(19, 19)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 28, 18, 0, 0), toTime(2019, 6, 28, 21, 0, 0), TemperatureForecast::Values(15.04, 15.04)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 28, 21, 0, 0), toTime(2019, 6, 29, 0, 0, 0), TemperatureForecast::Values(13.87, 13.87)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 29, 0, 0, 0), toTime(2019, 6, 29, 3, 0, 0), TemperatureForecast::Values(13.06, 13.06)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 29, 3, 0, 0), toTime(2019, 6, 29, 6, 0, 0), TemperatureForecast::Values(17.98, 17.98)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 29, 6, 0, 0), toTime(2019, 6, 29, 9, 0, 0), TemperatureForecast::Values(21.75, 21.75)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 29, 9, 0, 0), toTime(2019, 6, 29, 12, 0, 0), TemperatureForecast::Values(23.83, 23.83)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 29, 12, 0, 0), toTime(2019, 6, 29, 15, 0, 0), TemperatureForecast::Values(24.16, 24.16)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 29, 15, 0, 0), toTime(2019, 6, 29, 18, 0, 0), TemperatureForecast::Values(20.83, 20.83)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 29, 18, 0, 0), toTime(2019, 6, 29, 21, 0, 0), TemperatureForecast::Values(16.51, 16.51)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 29, 21, 0, 0), toTime(2019, 6, 30, 0, 0, 0), TemperatureForecast::Values(14.57, 14.57)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 30, 0, 0, 0), toTime(2019, 6, 30, 3, 0, 0), TemperatureForecast::Values(13.25, 13.25)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 30, 3, 0, 0), toTime(2019, 6, 30, 6, 0, 0), TemperatureForecast::Values(18.15, 18.15)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 30, 6, 0, 0), toTime(2019, 6, 30, 9, 0, 0), TemperatureForecast::Values(23.65, 23.65)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 30, 9, 0, 0), toTime(2019, 6, 30, 12, 0, 0), TemperatureForecast::Values(26.35, 26.35)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 30, 12, 0, 0), toTime(2019, 6, 30, 15, 0, 0), TemperatureForecast::Values(26.35, 26.35)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 30, 15, 0, 0), toTime(2019, 6, 30, 18, 0, 0), TemperatureForecast::Values(22.18, 22.18)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 30, 18, 0, 0), toTime(2019, 6, 30, 21, 0, 0), TemperatureForecast::Values(18.75, 18.75)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 6, 30, 21, 0, 0), toTime(2019, 7, 1, 0, 0, 0), TemperatureForecast::Values(17.25, 17.25)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 1, 0, 0, 0), toTime(2019, 7, 1, 3, 0, 0), TemperatureForecast::Values(16.67, 16.67)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 1, 3, 0, 0), toTime(2019, 7, 1, 6, 0, 0), TemperatureForecast::Values(20.75, 20.75)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 1, 6, 0, 0), toTime(2019, 7, 1, 9, 0, 0), TemperatureForecast::Values(25.44, 25.44)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 1, 9, 0, 0), toTime(2019, 7, 1, 12, 0, 0), TemperatureForecast::Values(28.15, 28.15)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 1, 12, 0, 0), toTime(2019, 7, 1, 15, 0, 0), TemperatureForecast::Values(27.86, 27.86)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 1, 15, 0, 0), toTime(2019, 7, 1, 18, 0, 0), TemperatureForecast::Values(23.24, 23.24)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 1, 18, 0, 0), toTime(2019, 7, 1, 21, 0, 0), TemperatureForecast::Values(20.45, 20.45)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 1, 21, 0, 0), toTime(2019, 7, 2, 0, 0, 0), TemperatureForecast::Values(19.05, 19.05)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 2, 0, 0, 0), toTime(2019, 7, 2, 3, 0, 0), TemperatureForecast::Values(17.85, 17.85)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 2, 3, 0, 0), toTime(2019, 7, 2, 6, 0, 0), TemperatureForecast::Values(22.14, 22.14)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 2, 6, 0, 0), toTime(2019, 7, 2, 9, 0, 0), TemperatureForecast::Values(25.35, 25.35)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 2, 9, 0, 0), toTime(2019, 7, 2, 12, 0, 0), TemperatureForecast::Values(26.24, 26.24)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 2, 12, 0, 0), toTime(2019, 7, 2, 15, 0, 0), TemperatureForecast::Values(24.93, 24.93)),
			TemperatureForecast::ValuesWithTimes(toTime(2019, 7, 2, 15, 0, 0), toTime(2019, 7, 2, 18, 0, 0), TemperatureForecast::Values(20.75, 20.75))
	}));
}

TEST(TemperatureForecastTest, updateCacheFromRealXml) {
	ifstream input("temperature_forecast.xml");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	auto mockTemperatureForecastReader = make_shared<MockTemperatureForecastReader>();

	EXPECT_CALL(*mockTemperatureForecastReader, read(_, _, _)).Times(1).WillOnce(Return(inputData.data()));

	TemperatureForecast temperatureForecast(mockTemperatureForecastReader);
	temperatureForecast.updateCache();

	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 27, 19, 0, 0)), Eq(TemperatureForecast::Values(18.25, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 27, 20, 0, 0)), Eq(TemperatureForecast::Values(18.25, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 27, 21, 0, 0)), Eq(TemperatureForecast::Values(18.25, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 27, 22, 0, 0)), Eq(TemperatureForecast::Values(15.54, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 27, 23, 0, 0)), Eq(TemperatureForecast::Values(15.54, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28,  0, 0, 0)), Eq(TemperatureForecast::Values(15.54, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28,  1, 0, 0)), Eq(TemperatureForecast::Values(13.47, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28,  2, 0, 0)), Eq(TemperatureForecast::Values(13.47, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28,  3, 0, 0)), Eq(TemperatureForecast::Values(13.47, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28,  4, 0, 0)), Eq(TemperatureForecast::Values(13.47, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28,  5, 0, 0)), Eq(TemperatureForecast::Values(13.47, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28,  6, 0, 0)), Eq(TemperatureForecast::Values(13.47, 19.85)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28,  7, 0, 0)), Eq(TemperatureForecast::Values(13.47, 20.95)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28,  8, 0, 0)), Eq(TemperatureForecast::Values(13.47, 20.95)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28,  9, 0, 0)), Eq(TemperatureForecast::Values(13.47, 20.95)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28, 10, 0, 0)), Eq(TemperatureForecast::Values(13.47, 23.35)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28, 11, 0, 0)), Eq(TemperatureForecast::Values(13.47, 23.35)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28, 12, 0, 0)), Eq(TemperatureForecast::Values(13.47, 23.35)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28, 13, 0, 0)), Eq(TemperatureForecast::Values(13.47, 23.35)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28, 14, 0, 0)), Eq(TemperatureForecast::Values(13.47, 23.35)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28, 15, 0, 0)), Eq(TemperatureForecast::Values(13.47, 23.35)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28, 16, 0, 0)), Eq(TemperatureForecast::Values(13.47, 23.35)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28, 17, 0, 0)), Eq(TemperatureForecast::Values(13.47, 23.35)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 27, 18, 0, 0), toTime(2019, 6, 28, 18, 0, 0)), Eq(TemperatureForecast::Values(13.47, 23.35)));

	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 29, 2, 59, 59), toTime(2019, 6, 29, 11, 59, 59)), Eq(TemperatureForecast::Values(13.06, 23.83)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 29, 3,  0,  0), toTime(2019, 6, 29, 11, 59, 59)), Eq(TemperatureForecast::Values(17.98, 23.83)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 29, 3,  0,  1), toTime(2019, 6, 29, 11, 59, 59)), Eq(TemperatureForecast::Values(17.98, 23.83)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 29, 2, 59, 59), toTime(2019, 6, 29, 12,  0,  0)), Eq(TemperatureForecast::Values(13.06, 23.83)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 29, 3,  0,  0), toTime(2019, 6, 29, 12,  0,  0)), Eq(TemperatureForecast::Values(17.98, 23.83)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 29, 3,  0,  1), toTime(2019, 6, 29, 12,  0,  0)), Eq(TemperatureForecast::Values(17.98, 23.83)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 29, 2, 59, 59), toTime(2019, 6, 29, 12,  0,  1)), Eq(TemperatureForecast::Values(13.06, 24.16)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 29, 3,  0,  0), toTime(2019, 6, 29, 12,  0,  1)), Eq(TemperatureForecast::Values(17.98, 24.16)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 6, 29, 3,  0,  1), toTime(2019, 6, 29, 12,  0,  1)), Eq(TemperatureForecast::Values(17.98, 24.16)));

	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 7, 1, 11, 59, 59), toTime(2019, 7, 1, 17, 59, 59)), Eq(TemperatureForecast::Values(23.24, 28.15)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 7, 1, 12,  0,  0), toTime(2019, 7, 1, 17, 59, 59)), Eq(TemperatureForecast::Values(23.24, 27.86)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 7, 1, 12,  0,  1), toTime(2019, 7, 1, 17, 59, 59)), Eq(TemperatureForecast::Values(23.24, 27.86)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 7, 1, 11, 59, 59), toTime(2019, 7, 1, 18,  0,  0)), Eq(TemperatureForecast::Values(23.24, 28.15)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 7, 1, 12,  0,  0), toTime(2019, 7, 1, 18,  0,  0)), Eq(TemperatureForecast::Values(23.24, 27.86)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 7, 1, 12,  0,  1), toTime(2019, 7, 1, 18,  0,  0)), Eq(TemperatureForecast::Values(23.24, 27.86)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 7, 1, 11, 59, 59), toTime(2019, 7, 1, 18,  0,  1)), Eq(TemperatureForecast::Values(20.45, 28.15)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 7, 1, 12,  0,  0), toTime(2019, 7, 1, 18,  0,  1)), Eq(TemperatureForecast::Values(20.45, 27.86)));
	EXPECT_THAT(temperatureForecast.getForecastValues(toTime(2019, 7, 1, 12,  0,  1), toTime(2019, 7, 1, 18,  0,  1)), Eq(TemperatureForecast::Values(20.45, 27.86)));
}
