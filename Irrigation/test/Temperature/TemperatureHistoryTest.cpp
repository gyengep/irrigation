#include <gmock/gmock.h>
#include <fstream>
#include "Temperature/TemperatureHistoryImpl.h"
#include "Temperature/TemperatureException.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"
#include "Mocks/MockCurrentTemperature.h"

using namespace std;
using namespace chrono;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureHistoryTest, updateCache) {
	auto mockCurrentTemperature = make_shared<MockCurrentTemperature>();
	TemperatureHistoryImpl temperatureHistory(mockCurrentTemperature, seconds(10));

	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).
			WillOnce(Return(20.0f)).
			WillOnce(Return(50.0f));

	temperatureHistory.updateCache(2);
	temperatureHistory.updateCache(5);

	EXPECT_THAT(temperatureHistory.getContainer(),
		ElementsAre(
			TemperatureHistoryImpl::TemperatureSample(2, 20.0f),
			TemperatureHistoryImpl::TemperatureSample(5, 50.0f)
		)
	);
}

TEST(TemperatureHistoryTest, removeNewer) {
	auto mockCurrentTemperature = make_shared<MockCurrentTemperature>();
	TemperatureHistoryImpl temperatureHistory(mockCurrentTemperature, seconds(10));

	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).
			WillOnce(Return(20.0f)).
			WillOnce(Return(30.0f)).
			WillOnce(Return(25.0f)).
			WillOnce(Return(35.0f)).
			WillOnce(Return(12.0f)).
			WillOnce(Return(10.0f));

	temperatureHistory.updateCache(2);
	temperatureHistory.updateCache(3);
	temperatureHistory.updateCache(4);
	temperatureHistory.updateCache(5);
	temperatureHistory.updateCache(3);

	EXPECT_THAT(temperatureHistory.getContainer(),
		ElementsAre(
			TemperatureHistoryImpl::TemperatureSample(2, 20.0f),
			TemperatureHistoryImpl::TemperatureSample(3, 12.0f)
		)
	);

	temperatureHistory.updateCache(1);

	EXPECT_THAT(temperatureHistory.getContainer(),
		ElementsAre(
			TemperatureHistoryImpl::TemperatureSample(1, 10.0f)
		)
	);
}

TEST(TemperatureHistoryTest, removeOlder) {
	auto mockCurrentTemperature = make_shared<MockCurrentTemperature>();
	TemperatureHistoryImpl temperatureHistory(mockCurrentTemperature, minutes(2));

	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).
			WillOnce(Return(124.0f)).
			WillOnce(Return(105.0f)).
			WillOnce(Return(85.0f)).
			WillOnce(Return(65.0f)).
			WillOnce(Return(45.0f)).
			WillOnce(Return(25.0f)).
			WillOnce(Return(5.0f)).
			WillOnce(Return(2.0f));

	temperatureHistory.updateCache(5);
	temperatureHistory.updateCache(6);
	temperatureHistory.updateCache(25);
	temperatureHistory.updateCache(45);
	temperatureHistory.updateCache(65);
	temperatureHistory.updateCache(85);
	temperatureHistory.updateCache(105);
	temperatureHistory.updateCache(125);

	EXPECT_THAT(temperatureHistory.getContainer(),
		ElementsAre(
			TemperatureHistoryImpl::TemperatureSample(6, 105.0f),
			TemperatureHistoryImpl::TemperatureSample(25, 85.0f),
			TemperatureHistoryImpl::TemperatureSample(45, 65.0f),
			TemperatureHistoryImpl::TemperatureSample(65, 45.0f),
			TemperatureHistoryImpl::TemperatureSample(85, 25.0f),
			TemperatureHistoryImpl::TemperatureSample(105, 5.0f),
			TemperatureHistoryImpl::TemperatureSample(125, 2.0f)
		)
	);
}

TEST(TemperatureHistoryTest, getHistoryMin) {
	auto mockCurrentTemperature = make_shared<MockCurrentTemperature>();
	TemperatureHistoryImpl temperatureHistory(mockCurrentTemperature, seconds(20));

	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).
			WillOnce(Return(10.0f)).
			WillOnce(Return(20.0f)).
			WillOnce(Return(30.0f)).
			WillOnce(Return(70.0f)).
			WillOnce(Return(80.0f)).
			WillOnce(Return(90.0f)).
			WillOnce(Return(130.0f)).
			WillOnce(Return(140.0f)).
			WillOnce(Return(150.0f));

	temperatureHistory.updateCache(1);
	temperatureHistory.updateCache(2);
	temperatureHistory.updateCache(3);
	temperatureHistory.updateCache(7);
	temperatureHistory.updateCache(8);
	temperatureHistory.updateCache(9);
	temperatureHistory.updateCache(13);
	temperatureHistory.updateCache(14);
	temperatureHistory.updateCache(15);

	EXPECT_THAT(temperatureHistory.getTemperatureHistory(2, 11).min, Eq(20));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(3, 11).min, Eq(30));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(4, 11).min, Eq(70));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 11).min, Eq(70));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 8).min,  Eq(70));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 9).min,  Eq(70));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 10).min, Eq(70));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 11).min, Eq(70));
}

TEST(TemperatureHistoryTest, getHistoryMax) {
	auto mockCurrentTemperature = make_shared<MockCurrentTemperature>();
	TemperatureHistoryImpl temperatureHistory(mockCurrentTemperature, seconds(20));

	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).
			WillOnce(Return(10.0f)).
			WillOnce(Return(20.0f)).
			WillOnce(Return(30.0f)).
			WillOnce(Return(70.0f)).
			WillOnce(Return(80.0f)).
			WillOnce(Return(90.0f)).
			WillOnce(Return(130.0f)).
			WillOnce(Return(140.0f)).
			WillOnce(Return(150.0f));

	temperatureHistory.updateCache(1);
	temperatureHistory.updateCache(2);
	temperatureHistory.updateCache(3);
	temperatureHistory.updateCache(7);
	temperatureHistory.updateCache(8);
	temperatureHistory.updateCache(9);
	temperatureHistory.updateCache(13);
	temperatureHistory.updateCache(14);
	temperatureHistory.updateCache(15);

	EXPECT_THAT(temperatureHistory.getTemperatureHistory(2, 11).max, Eq(90));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(3, 11).max, Eq(90));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(4, 11).max, Eq(90));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 11).max, Eq(90));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 8).max,  Eq(80));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 9).max,  Eq(90));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 10).max, Eq(90));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 11).max, Eq(90));
}

TEST(TemperatureHistoryTest, getHistoryAvg) {
	auto mockCurrentTemperature = make_shared<MockCurrentTemperature>();
	TemperatureHistoryImpl temperatureHistory(mockCurrentTemperature, seconds(20));

	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).
			WillOnce(Return(10.0f)).
			WillOnce(Return(20.0f)).
			WillOnce(Return(30.0f)).
			WillOnce(Return(70.0f)).
			WillOnce(Return(80.0f)).
			WillOnce(Return(90.0f)).
			WillOnce(Return(130.0f)).
			WillOnce(Return(140.0f)).
			WillOnce(Return(150.0f));

	temperatureHistory.updateCache(1);
	temperatureHistory.updateCache(2);
	temperatureHistory.updateCache(3);
	temperatureHistory.updateCache(7);
	temperatureHistory.updateCache(8);
	temperatureHistory.updateCache(9);
	temperatureHistory.updateCache(13);
	temperatureHistory.updateCache(14);
	temperatureHistory.updateCache(15);

	EXPECT_THAT(temperatureHistory.getTemperatureHistory(2, 11).avg, Eq(58));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(3, 11).avg, Eq(67.5));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(4, 11).avg, Eq(80));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 11).avg, Eq(80));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 8).avg,  Eq(75));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 9).avg,  Eq(80));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 10).avg, Eq(80));
	EXPECT_THAT(temperatureHistory.getTemperatureHistory(5, 11).avg, Eq(80));
}

TEST(TemperatureHistoryTest, getHistoryNotFound) {
	auto mockCurrentTemperature = make_shared<MockCurrentTemperature>();
	TemperatureHistoryImpl temperatureHistory(mockCurrentTemperature, seconds(10));

	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).
			WillOnce(Return(40.0f)).
			WillOnce(Return(60.0f));

	temperatureHistory.updateCache(4);
	temperatureHistory.updateCache(8);

	EXPECT_THROW(temperatureHistory.getTemperatureHistory(1, 3), TemperatureException);
	EXPECT_THROW(temperatureHistory.getTemperatureHistory(5, 6), TemperatureException);
	EXPECT_THROW(temperatureHistory.getTemperatureHistory(10, 15), TemperatureException);
}

TEST(TemperatureHistoryTest, save) {
	const char* fileName = tmpnam(nullptr);

	{
		auto mockCurrentTemperature = make_shared<MockCurrentTemperature>();
		TemperatureHistoryImpl temperatureHistory(
			mockCurrentTemperature,
			minutes(2),
			fileName,
			make_shared<CsvReaderImplFactory>(),
			make_shared<CsvWriterImplFactory>()
		);

		EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).
				WillOnce(Return(20.0f)).
				WillOnce(Return(40.0f)).
				WillOnce(Return(60.0f));

		temperatureHistory.updateCache(2);
		temperatureHistory.updateCache(4);
		temperatureHistory.updateCache(6);
	}

	ifstream input(fileName);
	vector<char> inputData(
	         (istreambuf_iterator<char>(input)),
	         (istreambuf_iterator<char>()));

	EXPECT_THAT(string(inputData.data(), inputData.size()), "2,20.0\n4,40.0\n6,60.0\n");
}

TEST(TemperatureHistoryTest, load) {
	const char* fileName = tmpnam(nullptr);

	{
		const string text = "1,15.1\n5,35.6\n";

		ofstream ofs(fileName);
		ofs.write(text.data(), text.size());
	}

	TemperatureHistoryImpl temperatureHistory(
		make_shared<MockCurrentTemperature>(),
		minutes(2),
		fileName,
		make_shared<CsvReaderImplFactory>(),
		make_shared<CsvWriterImplFactory>()
	);

	EXPECT_THAT(temperatureHistory.getContainer(),
		ElementsAre(
			TemperatureHistoryImpl::TemperatureSample(1, 15.1f),
			TemperatureHistoryImpl::TemperatureSample(5, 35.6f)
		)
	);
}

TEST(TemperatureHistoryTest, loadInvalidFile) {
	const char* fileName = tmpnam(nullptr);

	{
		const string text = "1,15.1,3\n5,35.6\n";

		ofstream ofs(fileName);
		ofs.write(text.data(), text.size());
	}

	TemperatureHistoryImpl temperatureHistory(
		make_shared<MockCurrentTemperature>(),
		minutes(2),
		fileName,
		make_shared<CsvReaderImplFactory>(),
		make_shared<CsvWriterImplFactory>()
	);

	EXPECT_THAT(temperatureHistory.getContainer(), IsEmpty());
}

TEST(TemperatureHistoryTest, loadNotExistingFile) {
	const char* fileName = tmpnam(nullptr);

	TemperatureHistoryImpl temperatureHistory(
		make_shared<MockCurrentTemperature>(),
		minutes(2),
		fileName,
		make_shared<CsvReaderImplFactory>(),
		make_shared<CsvWriterImplFactory>()
	);

	EXPECT_THAT(temperatureHistory.getContainer(), IsEmpty());
}

TEST(TemperatureHistoryTest, onTimer) {
	auto mockCurrentTemperature = make_shared<MockCurrentTemperature>();

	EXPECT_CALL(*mockCurrentTemperature, getCurrentTemperature()).
			WillOnce(Return(40.0f));

	TemperatureHistoryImpl temperatureHistory(mockCurrentTemperature, seconds(10));

	EXPECT_NO_THROW(temperatureHistory.onTimer());
}
