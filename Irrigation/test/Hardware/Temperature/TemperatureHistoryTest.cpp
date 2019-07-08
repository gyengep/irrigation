#include <gmock/gmock.h>
#include <fstream>
#include "Hardware/Temperature/TemperatureHistoryImpl.h"
#include "Exceptions/Exceptions.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"
#include "Mocks/MockTemperatureSensor.h"

using namespace std;
using namespace chrono;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TemperatureHistoryTest, updateCache) {
	auto mockTemperatureSensor = make_shared<MockTemperatureSensor>();
	TemperatureHistoryImpl temperatureHistory(mockTemperatureSensor, seconds(10));

	EXPECT_CALL(*mockTemperatureSensor, getCachedValue()).
			WillOnce(Return(20.0f)).
			WillOnce(Return(50.0f));

	temperatureHistory.updateCache(system_clock::from_time_t(2));
	temperatureHistory.updateCache(system_clock::from_time_t(5));

	EXPECT_THAT(temperatureHistory.getContainer(),
		ElementsAre(
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(2), 20.0f),
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(5), 50.0f)
		)
	);
}

TEST(TemperatureHistoryTest, removeNewer) {
	auto mockTemperatureSensor = make_shared<MockTemperatureSensor>();
	TemperatureHistoryImpl temperatureHistory(mockTemperatureSensor, seconds(10));

	EXPECT_CALL(*mockTemperatureSensor, getCachedValue()).
			WillOnce(Return(20.0f)).
			WillOnce(Return(30.0f)).
			WillOnce(Return(25.0f)).
			WillOnce(Return(35.0f)).
			WillOnce(Return(12.0f)).
			WillOnce(Return(10.0f));

	temperatureHistory.updateCache(system_clock::from_time_t(2));
	temperatureHistory.updateCache(system_clock::from_time_t(3));
	temperatureHistory.updateCache(system_clock::from_time_t(4));
	temperatureHistory.updateCache(system_clock::from_time_t(5));
	temperatureHistory.updateCache(system_clock::from_time_t(3));

	EXPECT_THAT(temperatureHistory.getContainer(),
		ElementsAre(
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(2), 20.0f),
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(3), 12.0f)
		)
	);

	temperatureHistory.updateCache(system_clock::from_time_t(1));

	EXPECT_THAT(temperatureHistory.getContainer(),
		ElementsAre(
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(1), 10.0f)
		)
	);
}

TEST(TemperatureHistoryTest, removeOlder) {
	auto mockTemperatureSensor = make_shared<MockTemperatureSensor>();
	TemperatureHistoryImpl temperatureHistory(mockTemperatureSensor, minutes(2));

	EXPECT_CALL(*mockTemperatureSensor, getCachedValue()).
			WillOnce(Return(124.0f)).
			WillOnce(Return(105.0f)).
			WillOnce(Return(85.0f)).
			WillOnce(Return(65.0f)).
			WillOnce(Return(45.0f)).
			WillOnce(Return(25.0f)).
			WillOnce(Return(5.0f)).
			WillOnce(Return(2.0f));

	temperatureHistory.updateCache(system_clock::from_time_t(5));
	temperatureHistory.updateCache(system_clock::from_time_t(6));
	temperatureHistory.updateCache(system_clock::from_time_t(25));
	temperatureHistory.updateCache(system_clock::from_time_t(45));
	temperatureHistory.updateCache(system_clock::from_time_t(65));
	temperatureHistory.updateCache(system_clock::from_time_t(85));
	temperatureHistory.updateCache(system_clock::from_time_t(105));
	temperatureHistory.updateCache(system_clock::from_time_t(125));

	EXPECT_THAT(temperatureHistory.getContainer(),
		ElementsAre(
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(6), 105.0f),
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(25), 85.0f),
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(45), 65.0f),
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(65), 45.0f),
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(85), 25.0f),
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(105), 5.0f),
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(125), 2.0f)
		)
	);
}

TEST(TemperatureHistoryTest, getHistoryMin) {
	auto mockTemperatureSensor = make_shared<MockTemperatureSensor>();
	TemperatureHistoryImpl temperatureHistory(mockTemperatureSensor, seconds(20));

	EXPECT_CALL(*mockTemperatureSensor, getCachedValue()).
			WillOnce(Return(10.0f)).
			WillOnce(Return(20.0f)).
			WillOnce(Return(30.0f)).
			WillOnce(Return(70.0f)).
			WillOnce(Return(80.0f)).
			WillOnce(Return(90.0f)).
			WillOnce(Return(130.0f)).
			WillOnce(Return(140.0f)).
			WillOnce(Return(150.0f));

	temperatureHistory.updateCache(system_clock::from_time_t(1));
	temperatureHistory.updateCache(system_clock::from_time_t(2));
	temperatureHistory.updateCache(system_clock::from_time_t(3));
	temperatureHistory.updateCache(system_clock::from_time_t(7));
	temperatureHistory.updateCache(system_clock::from_time_t(8));
	temperatureHistory.updateCache(system_clock::from_time_t(9));
	temperatureHistory.updateCache(system_clock::from_time_t(13));
	temperatureHistory.updateCache(system_clock::from_time_t(14));
	temperatureHistory.updateCache(system_clock::from_time_t(15));

	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(2), system_clock::from_time_t(11)).min, Eq(20));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(3), system_clock::from_time_t(11)).min, Eq(30));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(4), system_clock::from_time_t(11)).min, Eq(70));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(11)).min, Eq(70));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(8)).min,  Eq(70));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(9)).min,  Eq(70));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(10)).min, Eq(70));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(11)).min, Eq(70));
}

TEST(TemperatureHistoryTest, getHistoryMax) {
	auto mockTemperatureSensor = make_shared<MockTemperatureSensor>();
	TemperatureHistoryImpl temperatureHistory(mockTemperatureSensor, seconds(20));

	EXPECT_CALL(*mockTemperatureSensor, getCachedValue()).
			WillOnce(Return(10.0f)).
			WillOnce(Return(20.0f)).
			WillOnce(Return(30.0f)).
			WillOnce(Return(70.0f)).
			WillOnce(Return(80.0f)).
			WillOnce(Return(90.0f)).
			WillOnce(Return(130.0f)).
			WillOnce(Return(140.0f)).
			WillOnce(Return(150.0f));

	temperatureHistory.updateCache(system_clock::from_time_t(1));
	temperatureHistory.updateCache(system_clock::from_time_t(2));
	temperatureHistory.updateCache(system_clock::from_time_t(3));
	temperatureHistory.updateCache(system_clock::from_time_t(7));
	temperatureHistory.updateCache(system_clock::from_time_t(8));
	temperatureHistory.updateCache(system_clock::from_time_t(9));
	temperatureHistory.updateCache(system_clock::from_time_t(13));
	temperatureHistory.updateCache(system_clock::from_time_t(14));
	temperatureHistory.updateCache(system_clock::from_time_t(15));

	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(2), system_clock::from_time_t(11)).max, Eq(90));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(3), system_clock::from_time_t(11)).max, Eq(90));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(4), system_clock::from_time_t(11)).max, Eq(90));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(11)).max, Eq(90));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(8)).max,  Eq(80));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(9)).max,  Eq(90));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(10)).max, Eq(90));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(11)).max, Eq(90));
}

TEST(TemperatureHistoryTest, getHistoryAvg) {
	auto mockTemperatureSensor = make_shared<MockTemperatureSensor>();
	TemperatureHistoryImpl temperatureHistory(mockTemperatureSensor, seconds(20));

	EXPECT_CALL(*mockTemperatureSensor, getCachedValue()).
			WillOnce(Return(10.0f)).
			WillOnce(Return(20.0f)).
			WillOnce(Return(30.0f)).
			WillOnce(Return(70.0f)).
			WillOnce(Return(80.0f)).
			WillOnce(Return(90.0f)).
			WillOnce(Return(130.0f)).
			WillOnce(Return(140.0f)).
			WillOnce(Return(150.0f));

	temperatureHistory.updateCache(system_clock::from_time_t(1));
	temperatureHistory.updateCache(system_clock::from_time_t(2));
	temperatureHistory.updateCache(system_clock::from_time_t(3));
	temperatureHistory.updateCache(system_clock::from_time_t(7));
	temperatureHistory.updateCache(system_clock::from_time_t(8));
	temperatureHistory.updateCache(system_clock::from_time_t(9));
	temperatureHistory.updateCache(system_clock::from_time_t(13));
	temperatureHistory.updateCache(system_clock::from_time_t(14));
	temperatureHistory.updateCache(system_clock::from_time_t(15));

	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(2), system_clock::from_time_t(11)).avg, Eq(58));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(3), system_clock::from_time_t(11)).avg, Eq(67.5));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(4), system_clock::from_time_t(11)).avg, Eq(80));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(11)).avg, Eq(80));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(8)).avg,  Eq(75));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(9)).avg,  Eq(80));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(10)).avg, Eq(80));
	EXPECT_THAT(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(11)).avg, Eq(80));
}

TEST(TemperatureHistoryTest, getHistoryNotFound) {
	auto mockTemperatureSensor = make_shared<MockTemperatureSensor>();
	TemperatureHistoryImpl temperatureHistory(mockTemperatureSensor, seconds(10));

	EXPECT_CALL(*mockTemperatureSensor, getCachedValue()).
			WillOnce(Return(40.0f)).
			WillOnce(Return(60.0f));

	temperatureHistory.updateCache(system_clock::from_time_t(4));
	temperatureHistory.updateCache(system_clock::from_time_t(8));

	EXPECT_THROW(temperatureHistory.getHistoryValues(system_clock::from_time_t(1), system_clock::from_time_t(3)), NoSuchElementException);
	EXPECT_THROW(temperatureHistory.getHistoryValues(system_clock::from_time_t(5), system_clock::from_time_t(6)), NoSuchElementException);
	EXPECT_THROW(temperatureHistory.getHistoryValues(system_clock::from_time_t(10), system_clock::from_time_t(15)), NoSuchElementException);
}

TEST(TemperatureHistoryTest, save) {
	const char* fileName = tmpnam(nullptr);

	{
		auto mockTemperatureSensor = make_shared<MockTemperatureSensor>();
		TemperatureHistoryImpl temperatureHistory(
			mockTemperatureSensor,
			minutes(2),
			fileName,
			make_shared<CsvReaderImplFactory>(),
			make_shared<CsvWriterImplFactory>()
		);

		EXPECT_CALL(*mockTemperatureSensor, getCachedValue()).
				WillOnce(Return(20.0f)).
				WillOnce(Return(40.0f)).
				WillOnce(Return(60.0f));

		temperatureHistory.updateCache(system_clock::from_time_t(2));
		temperatureHistory.updateCache(system_clock::from_time_t(4));
		temperatureHistory.updateCache(system_clock::from_time_t(6));
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
		make_shared<MockTemperatureSensor>(),
		minutes(2),
		fileName,
		make_shared<CsvReaderImplFactory>(),
		make_shared<CsvWriterImplFactory>()
	);

	EXPECT_THAT(temperatureHistory.getContainer(),
		ElementsAre(
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(1), 15.1f),
			TemperatureHistoryImpl::TemperatureSample(system_clock::from_time_t(5), 35.6f)
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
		make_shared<MockTemperatureSensor>(),
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
		make_shared<MockTemperatureSensor>(),
		minutes(2),
		fileName,
		make_shared<CsvReaderImplFactory>(),
		make_shared<CsvWriterImplFactory>()
	);

	EXPECT_THAT(temperatureHistory.getContainer(), IsEmpty());
}

TEST(TemperatureHistoryTest, onTimer) {
	auto mockTemperatureSensor = make_shared<MockTemperatureSensor>();

	EXPECT_CALL(*mockTemperatureSensor, getCachedValue()).
			WillOnce(Return(40.0f));

	TemperatureHistoryImpl temperatureHistory(mockTemperatureSensor, seconds(10));

	EXPECT_NO_THROW(temperatureHistory.onTimer());
}
