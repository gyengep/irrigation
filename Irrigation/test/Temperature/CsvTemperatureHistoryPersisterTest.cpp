#include <gmock/gmock.h>
#include "Mocks/MockCsvReader.h"
#include "Mocks/MockCsvWriter.h"
#include "TestCommon/cout.h"
#include "Temperature/TemperatureHistoryPersister.h"
#include "Temperature/CsvTemperatureHistoryPersister.h"
#include "Temperature/TemperatureException.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(CsvTemperatureHistoryPersisterTest, add) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(AtLeast(1));
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(AtLeast(1));

	CsvTemperatureHistoryPersister temperatureHistoryPersister(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(2, 20.f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(5, 50.f));

	EXPECT_THAT(temperatureHistoryPersister.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryPersister::Sample(2, 20.0f),
			TemperatureHistoryPersister::Sample(5, 50.0f)
		)
	);
}

TEST(CsvTemperatureHistoryPersisterTest, addExisting) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(AtLeast(1));
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(AtLeast(1));

	CsvTemperatureHistoryPersister temperatureHistoryPersister(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(2, 20.f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(5, 50.f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(2, 30.f));

	EXPECT_THAT(temperatureHistoryPersister.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryPersister::Sample(2, 30.0f),
			TemperatureHistoryPersister::Sample(5, 50.0f)
		)
	);
}

TEST(CsvTemperatureHistoryPersisterTest, removeNewer) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(AtLeast(1));
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(AtLeast(1));

	CsvTemperatureHistoryPersister temperatureHistoryPersister(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(2, 20.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(3, 30.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(4, 25.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(5, 35.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(3, 12.0f));

	temperatureHistoryPersister.removeNewer(3);

	EXPECT_THAT(temperatureHistoryPersister.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryPersister::Sample(2, 20.0f),
			TemperatureHistoryPersister::Sample(3, 12.0f)
		)
	);
}

TEST(CsvTemperatureHistoryPersisterTest, removeOlder) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(AtLeast(1));
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(AtLeast(1));

	CsvTemperatureHistoryPersister temperatureHistoryPersister(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(5,  124.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(6,  105.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(25, 85.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(24, 65.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(65, 45.0f));

	temperatureHistoryPersister.removeOlder(25);

	EXPECT_THAT(temperatureHistoryPersister.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryPersister::Sample(25, 85.0f),
			TemperatureHistoryPersister::Sample(65, 45.0f)
		)
	);
}

TEST(CsvTemperatureHistoryPersisterTest, getBetween) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(AtLeast(1));
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(AtLeast(1));

	CsvTemperatureHistoryPersister temperatureHistoryPersister(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(1, 10.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(2, 20.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(3, 30.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(7, 70.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(8, 80.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(9, 90.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(13, 130.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(14, 140.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(15, 150.0f));

	EXPECT_THAT(temperatureHistoryPersister.getBetween(8, 14),
		UnorderedElementsAre(
			TemperatureHistoryPersister::Sample(8, 80.0f),
			TemperatureHistoryPersister::Sample(9, 90.0f),
			TemperatureHistoryPersister::Sample(13, 130.0f),
			TemperatureHistoryPersister::Sample(14, 140.0f)
		)
	);
}

TEST(CsvTemperatureHistoryPersisterTest, getBetweenNotExisting) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(AtLeast(1));
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(AtLeast(1));

	CsvTemperatureHistoryPersister temperatureHistoryPersister(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(1, 10.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(2, 20.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(3, 30.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(13, 130.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(14, 140.0f));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(15, 150.0f));

	EXPECT_THAT(temperatureHistoryPersister.getBetween(5, 10), IsEmpty());
}

///////////////////////////////////////////////////////////////////////////////

TEST(CsvTemperatureHistoryPersisterTest, load) {
	const auto line1 = std::vector<std::string>{ "123", "25" };
	const auto line2 = std::vector<std::string>{ "124", "30" };
	const auto line3 = std::vector<std::string>{ "125", "35" };

	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).
			Times(4).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line1))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line2))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line3))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>())));
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(AtLeast(1));

	CsvTemperatureHistoryPersister temperatureHistoryPersister(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	EXPECT_THAT(temperatureHistoryPersister.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryPersister::Sample(123, 25.0f),
			TemperatureHistoryPersister::Sample(124, 30.0f),
			TemperatureHistoryPersister::Sample(125, 35.0f)
		)
	);
}

TEST(CsvTemperatureHistoryPersisterTest, loadInvalid) {
	const auto line1 = std::vector<std::string>{ "abc" };
	const auto line2 = std::vector<std::string>{ "124", "30", "1234" };
	const auto line3 = std::vector<std::string>{ "125", "35" };

	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).
			Times(4).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line1))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line2))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line3))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>())));
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(AtLeast(1));

	CsvTemperatureHistoryPersister temperatureHistoryPersister(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	EXPECT_THAT(temperatureHistoryPersister.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryPersister::Sample(125, 35.0f)
		)
	);
}

TEST(CsvTemperatureHistoryPersisterTest, save) {
	const auto line1 = std::vector<std::string>{ "123", "25.0" };
	const auto line2 = std::vector<std::string>{ "124", "30.0" };
	const auto line3 = std::vector<std::string>{ "125", "35.0" };

	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(AtLeast(1));
	EXPECT_CALL(*mockCsvWriter, append(line1)).Times(1);
	EXPECT_CALL(*mockCsvWriter, append(line2)).Times(1);
	EXPECT_CALL(*mockCsvWriter, append(line3)).Times(1);

	CsvTemperatureHistoryPersister temperatureHistoryPersister(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(123, 25));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(124, 30));
	temperatureHistoryPersister.add(TemperatureHistoryPersister::Sample(125, 35));
}

