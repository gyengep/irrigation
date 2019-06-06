#include <gmock/gmock.h>
#include <fstream>
#include "Hardware/Temperature/TemperatureStatistics.h"
#include "Hardware/Temperature/TemperatureSensorFake.h"
#include "Exceptions/Exceptions.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureStatisticsTest, addTempearture) {
	TemperatureStatistics temperatureStatistics(chrono::seconds(10));

	temperatureStatistics.addTemperature(2, 20.0f);
	temperatureStatistics.addTemperature(5, 50.0f);

	EXPECT_THAT(temperatureStatistics.getSamples(),
		ContainerEq(deque<TemperatureStatistics::TemperatureSample>{
			TemperatureStatistics::TemperatureSample(2, 20.0f),
			TemperatureStatistics::TemperatureSample(5, 50.0f)
		})
	);
}

TEST(TemperatureStatisticsTest, removeNewer) {
	TemperatureStatistics temperatureStatistics(chrono::seconds(10));

	temperatureStatistics.addTemperature(2, 20.0f);
	temperatureStatistics.addTemperature(3, 30.0f);
	temperatureStatistics.addTemperature(4, 40.0f);
	temperatureStatistics.addTemperature(5, 50.0f);
	temperatureStatistics.addTemperature(3, 130.0f);

	EXPECT_THAT(temperatureStatistics.getSamples(),
		ContainerEq(deque<TemperatureStatistics::TemperatureSample>{
			TemperatureStatistics::TemperatureSample(2, 20.0f),
			TemperatureStatistics::TemperatureSample(3, 130.0f)
		})
	);

	temperatureStatistics.addTemperature(1, 10.0f);

	EXPECT_THAT(temperatureStatistics.getSamples(),
		ContainerEq(deque<TemperatureStatistics::TemperatureSample>{
			TemperatureStatistics::TemperatureSample(1, 10.0f),
		})
	);
}

TEST(TemperatureStatisticsTest, removeOlder) {
	TemperatureStatistics temperatureStatistics(chrono::minutes(2));

	temperatureStatistics.addTemperature(5, 125.0f);
	temperatureStatistics.addTemperature(6, 124.0f);
	temperatureStatistics.addTemperature(25, 105.0f);
	temperatureStatistics.addTemperature(45, 85.0f);
	temperatureStatistics.addTemperature(65, 65.0f);
	temperatureStatistics.addTemperature(85, 45.0f);
	temperatureStatistics.addTemperature(105, 25.0f);
	temperatureStatistics.addTemperature(125, 5.0f);

	EXPECT_THAT(temperatureStatistics.getSamples(),
		ContainerEq(deque<TemperatureStatistics::TemperatureSample>{
			TemperatureStatistics::TemperatureSample(6, 124.0f),
			TemperatureStatistics::TemperatureSample(25, 105.0f),
			TemperatureStatistics::TemperatureSample(45, 85.0f),
			TemperatureStatistics::TemperatureSample(65, 65.0f),
			TemperatureStatistics::TemperatureSample(85, 45.0f),
			TemperatureStatistics::TemperatureSample(105, 25.0f),
			TemperatureStatistics::TemperatureSample(125, 5.0f),
		})
	);
}

TEST(TemperatureStatisticsTest, getStatisticsMin) {
	TemperatureStatistics temperatureStatistics(chrono::seconds(10));

	temperatureStatistics.addTemperature(2, 20.0f);
	temperatureStatistics.addTemperature(4, 40.0f);
	temperatureStatistics.addTemperature(6, 60.0f);
	temperatureStatistics.addTemperature(8, 80.0f);
	temperatureStatistics.addTemperature(10, 100.0f);

	EXPECT_THAT(temperatureStatistics.getStatistics(1, 9).minTemperature, Eq(20));
	EXPECT_THAT(temperatureStatistics.getStatistics(3, 9).minTemperature, Eq(40));
	EXPECT_THAT(temperatureStatistics.getStatistics(4, 9).minTemperature, Eq(40));
	EXPECT_THAT(temperatureStatistics.getStatistics(4, 8).minTemperature, Eq(40));
}

TEST(TemperatureStatisticsTest, getStatisticsMax) {
	TemperatureStatistics temperatureStatistics(chrono::seconds(10));

	temperatureStatistics.addTemperature(2, 20.0f);
	temperatureStatistics.addTemperature(4, 40.0f);
	temperatureStatistics.addTemperature(6, 60.0f);
	temperatureStatistics.addTemperature(8, 80.0f);
	temperatureStatistics.addTemperature(10, 100.0f);

	EXPECT_THAT(temperatureStatistics.getStatistics(1, 9).maxTemperature, Eq(80));
	EXPECT_THAT(temperatureStatistics.getStatistics(1, 7).maxTemperature, Eq(60));
	EXPECT_THAT(temperatureStatistics.getStatistics(2, 7).maxTemperature, Eq(60));
	EXPECT_THAT(temperatureStatistics.getStatistics(3, 6).maxTemperature, Eq(60));
}

TEST(TemperatureStatisticsTest, getStatisticsAvg) {
	TemperatureStatistics temperatureStatistics(chrono::seconds(10));

	temperatureStatistics.addTemperature(2, 20.0f);
	temperatureStatistics.addTemperature(4, 40.0f);
	temperatureStatistics.addTemperature(6, 60.0f);
	temperatureStatistics.addTemperature(8, 80.0f);
	temperatureStatistics.addTemperature(10, 100.0f);

	EXPECT_THAT(temperatureStatistics.getStatistics(1, 9).avgTemperature, Eq(50));
	EXPECT_THAT(temperatureStatistics.getStatistics(1, 7).avgTemperature, Eq(40));
	EXPECT_THAT(temperatureStatistics.getStatistics(2, 7).avgTemperature, Eq(40));
	EXPECT_THAT(temperatureStatistics.getStatistics(3, 6).avgTemperature, Eq(50));
}

TEST(TemperatureStatisticsTest, getStatisticsNotFound) {
	TemperatureStatistics temperatureStatistics(chrono::seconds(10));

	temperatureStatistics.addTemperature(4, 40.0f);
	temperatureStatistics.addTemperature(8, 60.0f);

	EXPECT_THROW(temperatureStatistics.getStatistics(1, 3), NoSuchElementException);
	EXPECT_THROW(temperatureStatistics.getStatistics(5, 6), NoSuchElementException);
	EXPECT_THROW(temperatureStatistics.getStatistics(10, 15), NoSuchElementException);
}

TEST(TemperatureStatisticsTest, save) {
	const char* fileName = tmpnam(nullptr);

	{
		TemperatureStatistics temperatureStatistics(
			chrono::minutes(2),
			fileName,
			make_shared<CsvReaderImplFactory>(),
			make_shared<CsvWriterImplFactory>()
		);

		temperatureStatistics.addTemperature(2, 20.0f);
		temperatureStatistics.addTemperature(4, 40.0f);
		temperatureStatistics.addTemperature(6, 60.0f);
	}

	ifstream input(fileName);
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	EXPECT_THAT(string(inputData.data(), inputData.size()), "2,20.0\n4,40.0\n6,60.0\n");
}

TEST(TemperatureStatisticsTest, load) {
	const char* fileName = tmpnam(nullptr);

	{
		const string text = "1,15.1\n5,35.6\n";

		ofstream ofs(fileName);
		ofs.write(text.data(), text.size());
	}

	TemperatureStatistics temperatureStatistics(
		chrono::minutes(2),
		fileName,
		make_shared<CsvReaderImplFactory>(),
		make_shared<CsvWriterImplFactory>()
	);

	EXPECT_THAT(temperatureStatistics.getSamples(),
		ContainerEq(deque<TemperatureStatistics::TemperatureSample>{
			TemperatureStatistics::TemperatureSample(1, 15.1f),
			TemperatureStatistics::TemperatureSample(5, 35.6f)
		})
	);
}

