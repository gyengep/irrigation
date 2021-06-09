#include <gmock/gmock.h>
#include <fstream>
#include <stdexcept>
#include "Mocks/MockNetworkReader.h"
#include "Temperature/DarkSkyWrapper.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(DarkSkyHandler_TemperatureForecastTest, parseJson) {
	ifstream input("DarkSky_forecast.json");
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	EXPECT_THAT(DarkSkyWrapper::parseTemperatureForecastJson(string(inputData.data(), inputData.size())), ElementsAreArray({
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563278400), DateTime(1563282000), 26.48, 26.48),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563282000), DateTime(1563285600), 27.20, 27.20),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563285600), DateTime(1563289200), 28.39, 28.39),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563289200), DateTime(1563292800), 29.13, 29.13),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563292800), DateTime(1563296400), 28.33, 28.33),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563296400), DateTime(1563300000), 27.39, 27.39),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563300000), DateTime(1563303600), 25.92, 25.92),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563303600), DateTime(1563307200), 24.76, 24.76),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563307200), DateTime(1563310800), 23.44, 23.44),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563310800), DateTime(1563314400), 22.25, 22.25),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563314400), DateTime(1563318000), 21.11, 21.11),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563318000), DateTime(1563321600), 20.35, 20.35),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563321600), DateTime(1563325200), 19.68, 19.68),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563325200), DateTime(1563328800), 18.63, 18.63),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563328800), DateTime(1563332400), 17.25, 17.25),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563332400), DateTime(1563336000), 16.62, 16.62),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563336000), DateTime(1563339600), 17.05, 17.05),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563339600), DateTime(1563343200), 18.23, 18.23),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563343200), DateTime(1563346800), 19.82, 19.82),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563346800), DateTime(1563350400), 21.40, 21.40),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563350400), DateTime(1563354000), 23.16, 23.16),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563354000), DateTime(1563357600), 24.47, 24.47),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563357600), DateTime(1563361200), 25.54, 25.54),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563361200), DateTime(1563364800), 26.44, 26.44),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563364800), DateTime(1563368400), 27.53, 27.53),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563368400), DateTime(1563372000), 28.31, 28.31),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563372000), DateTime(1563375600), 28.77, 28.77),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563375600), DateTime(1563379200), 28.90, 28.90),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563379200), DateTime(1563382800), 28.43, 28.43),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563382800), DateTime(1563386400), 27.05, 27.05),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563386400), DateTime(1563390000), 25.62, 25.62),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563390000), DateTime(1563393600), 23.69, 23.69),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563393600), DateTime(1563397200), 21.91, 21.91),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563397200), DateTime(1563400800), 20.38, 20.38),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563400800), DateTime(1563404400), 19.39, 19.39),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563404400), DateTime(1563408000), 18.84, 18.84),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563408000), DateTime(1563411600), 18.36, 18.36),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563411600), DateTime(1563415200), 17.76, 17.76),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563415200), DateTime(1563418800), 17.24, 17.24),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563418800), DateTime(1563422400), 16.98, 16.98),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563422400), DateTime(1563426000), 17.24, 17.24),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563426000), DateTime(1563429600), 18.19, 18.19),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563429600), DateTime(1563433200), 19.56, 19.56),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563433200), DateTime(1563436800), 21.14, 21.14),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563436800), DateTime(1563440400), 22.84, 22.84),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563440400), DateTime(1563444000), 24.50, 24.50),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563444000), DateTime(1563447600), 25.90, 25.90),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563447600), DateTime(1563451200), 27.46, 27.46),
			TemperatureForecastProvider::ValuesWithTimes(DateTime(1563451200), DateTime(1563454800), 28.33, 28.33)
	}));
}

TEST(DarkSkyHandler_TemperatureForecastTest, parseXmlInvalid) {
	EXPECT_ANY_THROW(DarkSkyWrapper::parseTemperatureForecastJson("abc"));
}

TEST(DarkSkyHandler_TemperatureForecastTest, parseXmlEmpty) {
	EXPECT_THAT(DarkSkyWrapper::parseTemperatureForecastJson("{ \"hourly\": { \"data\": []}}"), IsEmpty());
}

TEST(DarkSkyHandler_TemperatureForecastTest, getForecast) {

	const string json =
			"{ "
				"\"hourly\": {"
					"\"data\": ["
						"{\"time\": 1563278400, \"temperature\": 15.26},"
						"{\"time\": 1563282000, \"temperature\": 27.45},"
						"{\"time\": 1563285600, \"temperature\": 38.12}"
					"]"
				"}"
			"}";
	const list<TemperatureForecastProvider::ValuesWithTimes> expectedList {
		TemperatureForecastProvider::ValuesWithTimes(DateTime(1563278400), DateTime(1563282000), 15.26, 15.26),
		TemperatureForecastProvider::ValuesWithTimes(DateTime(1563282000), DateTime(1563285600), 27.45, 27.45),
		TemperatureForecastProvider::ValuesWithTimes(DateTime(1563285600), DateTime(1563289200), 38.12, 38.12)
	};

	auto mockNetworkReader = make_shared<MockNetworkReader>();
	EXPECT_CALL(*mockNetworkReader, read(_)).Times(1).WillOnce(Return(json));

	DarkSkyWrapper temperatureForecast(mockNetworkReader);
	EXPECT_THAT(temperatureForecast.readTemperatureForecast(), ContainerEq(expectedList));
}

TEST(DarkSkyHandler_TemperatureForecastTest, getForecastException) {

	auto mockNetworkReader = make_shared<MockNetworkReader>();
	EXPECT_CALL(*mockNetworkReader, read(_)).Times(1).WillOnce(Throw(exception()));

	DarkSkyWrapper temperatureForecast(mockNetworkReader);
	EXPECT_THROW(temperatureForecast.readTemperatureForecast(), exception);
}
