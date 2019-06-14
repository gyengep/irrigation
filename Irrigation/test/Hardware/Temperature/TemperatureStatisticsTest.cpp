#include <gmock/gmock.h>
#include <fstream>
#include "Hardware/Temperature/TemperatureStatisticsImpl.h"
#include "Exceptions/Exceptions.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureStatisticsImplTest, addTempearture) {
	TemperatureStatisticsImpl temperatureStatistics(chrono::seconds(10));

	temperatureStatistics.addTemperature(2, 20.0f);
	temperatureStatistics.addTemperature(5, 50.0f);

	EXPECT_THAT(temperatureStatistics.getSamples(),
		ContainerEq(deque<TemperatureStatisticsImpl::TemperatureSample>{
			TemperatureStatisticsImpl::TemperatureSample(2, 20.0f),
			TemperatureStatisticsImpl::TemperatureSample(5, 50.0f)
		})
	);
}

TEST(TemperatureStatisticsImplTest, removeNewer) {
	TemperatureStatisticsImpl temperatureStatistics(chrono::seconds(10));

	temperatureStatistics.addTemperature(2, 20.0f);
	temperatureStatistics.addTemperature(3, 30.0f);
	temperatureStatistics.addTemperature(4, 40.0f);
	temperatureStatistics.addTemperature(5, 50.0f);
	temperatureStatistics.addTemperature(3, 130.0f);

	EXPECT_THAT(temperatureStatistics.getSamples(),
		ContainerEq(deque<TemperatureStatisticsImpl::TemperatureSample>{
			TemperatureStatisticsImpl::TemperatureSample(2, 20.0f),
			TemperatureStatisticsImpl::TemperatureSample(3, 130.0f)
		})
	);

	temperatureStatistics.addTemperature(1, 10.0f);

	EXPECT_THAT(temperatureStatistics.getSamples(),
		ContainerEq(deque<TemperatureStatisticsImpl::TemperatureSample>{
			TemperatureStatisticsImpl::TemperatureSample(1, 10.0f),
		})
	);
}

TEST(TemperatureStatisticsImplTest, removeOlder) {
	TemperatureStatisticsImpl temperatureStatistics(chrono::minutes(2));

	temperatureStatistics.addTemperature(5, 125.0f);
	temperatureStatistics.addTemperature(6, 124.0f);
	temperatureStatistics.addTemperature(25, 105.0f);
	temperatureStatistics.addTemperature(45, 85.0f);
	temperatureStatistics.addTemperature(65, 65.0f);
	temperatureStatistics.addTemperature(85, 45.0f);
	temperatureStatistics.addTemperature(105, 25.0f);
	temperatureStatistics.addTemperature(125, 5.0f);

	EXPECT_THAT(temperatureStatistics.getSamples(),
		ContainerEq(deque<TemperatureStatisticsImpl::TemperatureSample>{
			TemperatureStatisticsImpl::TemperatureSample(6, 124.0f),
			TemperatureStatisticsImpl::TemperatureSample(25, 105.0f),
			TemperatureStatisticsImpl::TemperatureSample(45, 85.0f),
			TemperatureStatisticsImpl::TemperatureSample(65, 65.0f),
			TemperatureStatisticsImpl::TemperatureSample(85, 45.0f),
			TemperatureStatisticsImpl::TemperatureSample(105, 25.0f),
			TemperatureStatisticsImpl::TemperatureSample(125, 5.0f),
		})
	);
}

TEST(TemperatureStatisticsImplTest, getStatisticsMin) {
	TemperatureStatisticsImpl temperatureStatistics(chrono::seconds(20));

	temperatureStatistics.addTemperature(1, 10.0f);
	temperatureStatistics.addTemperature(2, 20.0f);
	temperatureStatistics.addTemperature(3, 30.0f);
	temperatureStatistics.addTemperature(7, 70.0f);
	temperatureStatistics.addTemperature(8, 80.0f);
	temperatureStatistics.addTemperature(9, 90.0f);
	temperatureStatistics.addTemperature(13, 130.0f);
	temperatureStatistics.addTemperature(14, 140.0f);
	temperatureStatistics.addTemperature(15, 150.0f);

	EXPECT_THAT(temperatureStatistics.getStatistics(2, 11).minTemperature, Eq(20));
	EXPECT_THAT(temperatureStatistics.getStatistics(3, 11).minTemperature, Eq(30));
	EXPECT_THAT(temperatureStatistics.getStatistics(4, 11).minTemperature, Eq(70));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 11).minTemperature, Eq(70));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 8).minTemperature, Eq(70));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 9).minTemperature, Eq(70));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 10).minTemperature, Eq(70));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 11).minTemperature, Eq(70));
}

TEST(TemperatureStatisticsImplTest, getStatisticsMax) {
	TemperatureStatisticsImpl temperatureStatistics(chrono::seconds(20));

	temperatureStatistics.addTemperature(1, 10.0f);
	temperatureStatistics.addTemperature(2, 20.0f);
	temperatureStatistics.addTemperature(3, 30.0f);
	temperatureStatistics.addTemperature(7, 70.0f);
	temperatureStatistics.addTemperature(8, 80.0f);
	temperatureStatistics.addTemperature(9, 90.0f);
	temperatureStatistics.addTemperature(13, 130.0f);
	temperatureStatistics.addTemperature(14, 140.0f);
	temperatureStatistics.addTemperature(15, 150.0f);

	EXPECT_THAT(temperatureStatistics.getStatistics(2, 11).maxTemperature, Eq(90));
	EXPECT_THAT(temperatureStatistics.getStatistics(3, 11).maxTemperature, Eq(90));
	EXPECT_THAT(temperatureStatistics.getStatistics(4, 11).maxTemperature, Eq(90));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 11).maxTemperature, Eq(90));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 8).maxTemperature, Eq(80));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 9).maxTemperature, Eq(90));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 10).maxTemperature, Eq(90));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 11).maxTemperature, Eq(90));
}

TEST(TemperatureStatisticsImplTest, getStatisticsAvg) {
	TemperatureStatisticsImpl temperatureStatistics(chrono::seconds(20));

	temperatureStatistics.addTemperature(1, 10.0f);
	temperatureStatistics.addTemperature(2, 20.0f);
	temperatureStatistics.addTemperature(3, 30.0f);
	temperatureStatistics.addTemperature(7, 70.0f);
	temperatureStatistics.addTemperature(8, 80.0f);
	temperatureStatistics.addTemperature(9, 90.0f);
	temperatureStatistics.addTemperature(13, 130.0f);
	temperatureStatistics.addTemperature(14, 140.0f);
	temperatureStatistics.addTemperature(15, 150.0f);

	EXPECT_THAT(temperatureStatistics.getStatistics(2, 11).avgTemperature, Eq(58));
	EXPECT_THAT(temperatureStatistics.getStatistics(3, 11).avgTemperature, Eq(67.5));
	EXPECT_THAT(temperatureStatistics.getStatistics(4, 11).avgTemperature, Eq(80));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 11).avgTemperature, Eq(80));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 8).avgTemperature, Eq(75));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 9).avgTemperature, Eq(80));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 10).avgTemperature, Eq(80));
	EXPECT_THAT(temperatureStatistics.getStatistics(5, 11).avgTemperature, Eq(80));
}

TEST(TemperatureStatisticsImplTest, getStatisticsNotFound) {
	TemperatureStatisticsImpl temperatureStatistics(chrono::seconds(10));

	temperatureStatistics.addTemperature(4, 40.0f);
	temperatureStatistics.addTemperature(8, 60.0f);

	EXPECT_THROW(temperatureStatistics.getStatistics(1, 3), NoSuchElementException);
	EXPECT_THROW(temperatureStatistics.getStatistics(5, 6), NoSuchElementException);
	EXPECT_THROW(temperatureStatistics.getStatistics(10, 15), NoSuchElementException);
}

TEST(TemperatureStatisticsImplTest, save) {
	const char* fileName = tmpnam(nullptr);

	{
		TemperatureStatisticsImpl temperatureStatistics(
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

TEST(TemperatureStatisticsImplTest, load) {
	const char* fileName = tmpnam(nullptr);

	{
		const string text = "1,15.1\n5,35.6\n";

		ofstream ofs(fileName);
		ofs.write(text.data(), text.size());
	}

	TemperatureStatisticsImpl temperatureStatistics(
		chrono::minutes(2),
		fileName,
		make_shared<CsvReaderImplFactory>(),
		make_shared<CsvWriterImplFactory>()
	);

	EXPECT_THAT(temperatureStatistics.getSamples(),
		ContainerEq(deque<TemperatureStatisticsImpl::TemperatureSample>{
			TemperatureStatisticsImpl::TemperatureSample(1, 15.1f),
			TemperatureStatisticsImpl::TemperatureSample(5, 35.6f)
		})
	);
}

TEST(TemperatureStatisticsImplTest, loadInvalidFile) {
	const char* fileName = tmpnam(nullptr);

	{
		const string text = "1,15.1,3\n5,35.6\n";

		ofstream ofs(fileName);
		ofs.write(text.data(), text.size());
	}

	TemperatureStatisticsImpl temperatureStatistics(
		chrono::minutes(2),
		fileName,
		make_shared<CsvReaderImplFactory>(),
		make_shared<CsvWriterImplFactory>()
	);

	EXPECT_THAT(temperatureStatistics.getSamples(), IsEmpty());
}

TEST(TemperatureStatisticsImplTest, loadNotExistingFile) {
	const char* fileName = tmpnam(nullptr);

	TemperatureStatisticsImpl temperatureStatistics(
		chrono::minutes(2),
		fileName,
		make_shared<CsvReaderImplFactory>(),
		make_shared<CsvWriterImplFactory>()
	);

	EXPECT_THAT(temperatureStatistics.getSamples(), IsEmpty());
}

