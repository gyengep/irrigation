#include <gmock/gmock.h>
#include "Mocks/MockCsvReader.h"
#include "Mocks/MockCsvWriter.h"
#include "TestCommon/cout.h"
#include "Temperature/TemperatureHistoryRepository.h"
#include "Temperature/CsvTemperatureHistoryRepository.h"
#include "Temperature/TemperatureException.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(CsvTemperatureHistoryRepositoryTest, add) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(0);
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(0);

	CsvTemperatureHistoryRepository temperatureHistoryRepository(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 2), 20.f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 5), 50.f));

	EXPECT_THAT(temperatureHistoryRepository.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 2), 20.0f),
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 5), 50.0f)
		)
	);
}

TEST(CsvTemperatureHistoryRepositoryTest, addExisting) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(0);
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(0);

	CsvTemperatureHistoryRepository temperatureHistoryRepository(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 2), 20.f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 5), 50.f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 2), 30.f));

	EXPECT_THAT(temperatureHistoryRepository.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 2), 30.0f),
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 5), 50.0f)
		)
	);
}

TEST(CsvTemperatureHistoryRepositoryTest, removeNewer) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(0);
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(0);

	CsvTemperatureHistoryRepository temperatureHistoryRepository(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 2), 20.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 3), 30.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 4), 25.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 5), 35.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 3), 12.0f));

	temperatureHistoryRepository.removeNewer(LocalDateTime::create(2021, 6, 1, 22, 33, 3));

	EXPECT_THAT(temperatureHistoryRepository.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 2), 20.0f),
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 3), 12.0f)
		)
	);
}

TEST(CsvTemperatureHistoryRepositoryTest, removeOlder) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(0);
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(0);

	CsvTemperatureHistoryRepository temperatureHistoryRepository(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 5),  124.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 6),  105.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 25), 85.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 24), 65.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 34, 5), 45.0f));

	temperatureHistoryRepository.removeOlder(LocalDateTime::create(2021, 6, 1, 22, 33, 25));

	EXPECT_THAT(temperatureHistoryRepository.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 25), 85.0f),
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 34, 5), 45.0f)
		)
	);
}

TEST(CsvTemperatureHistoryRepositoryTest, getBetween) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(0);
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(0);

	CsvTemperatureHistoryRepository temperatureHistoryRepository(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 1), 10.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 2), 20.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 3), 30.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 7), 70.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 8), 80.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 9), 90.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 13), 130.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 14), 140.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 15), 150.0f));

	EXPECT_THAT(
		temperatureHistoryRepository.getBetween(
				LocalDateTime::create(2021, 6, 1, 22, 33, 8),
				LocalDateTime::create(2021, 6, 1, 22, 33, 14)),
		UnorderedElementsAre(
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 8), 80.0f),
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 9), 90.0f),
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 13), 130.0f),
			TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 14), 140.0f)
		)
	);
}

TEST(CsvTemperatureHistoryRepositoryTest, getBetweenNotExisting) {
	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(0);
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(0);

	CsvTemperatureHistoryRepository temperatureHistoryRepository(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 1), 10.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 2), 20.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 3), 30.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 13), 130.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 14), 140.0f));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(LocalDateTime::create(2021, 6, 1, 22, 33, 15), 150.0f));

	EXPECT_THAT(
			temperatureHistoryRepository.getBetween(
					LocalDateTime::create(2021, 6, 1, 22, 33, 5),
					LocalDateTime::create(2021, 6, 1, 22, 33, 10)),
			IsEmpty()
		);
}

///////////////////////////////////////////////////////////////////////////////

TEST(CsvTemperatureHistoryRepositoryTest, load) {
	const auto line1 = std::vector<std::string>{ "1622580963", "25" };
	const auto line2 = std::vector<std::string>{ "1622580964", "30" };
	const auto line3 = std::vector<std::string>{ "1622580965", "35" };

	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).
			Times(4).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line1))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line2))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line3))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>())));
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(0);

	CsvTemperatureHistoryRepository temperatureHistoryRepository(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryRepository.load();

	EXPECT_THAT(temperatureHistoryRepository.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryRepository::Sample(UtcDateTime::create(2021, 6, 1, 20, 56, 3), 25.0f),
			TemperatureHistoryRepository::Sample(UtcDateTime::create(2021, 6, 1, 20, 56, 4), 30.0f),
			TemperatureHistoryRepository::Sample(UtcDateTime::create(2021, 6, 1, 20, 56, 5), 35.0f)
		)
	);
}

TEST(CsvTemperatureHistoryRepositoryTest, loadInvalid) {
	const auto line1 = std::vector<std::string>{ "abc" };
	const auto line2 = std::vector<std::string>{ "1622580962", "30", "1234" };
	const auto line3 = std::vector<std::string>{ "1622580963", "35" };

	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).
			Times(4).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line1))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line2))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>(line3))))).
			WillOnce(Return(ByMove(std::unique_ptr<std::vector<std::string>>())));
	EXPECT_CALL(*mockCsvWriter, append(_)).Times(0);

	CsvTemperatureHistoryRepository temperatureHistoryRepository(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryRepository.load();

	EXPECT_THAT(temperatureHistoryRepository.getAll(),
		UnorderedElementsAre(
			TemperatureHistoryRepository::Sample(UtcDateTime::create(2021, 6, 1, 20, 56, 3), 35.0f)
		)
	);
}

TEST(CsvTemperatureHistoryRepositoryTest, save) {
	const auto line1 = std::vector<std::string>{ "1622580963", "25.0" };
	const auto line2 = std::vector<std::string>{ "1622580964", "30.0" };
	const auto line3 = std::vector<std::string>{ "1622580965", "35.0" };

	auto mockCsvReader = std::make_shared<MockCsvReader>();
	auto mockCsvWriter = std::make_shared<MockCsvWriter>();

	EXPECT_CALL(*mockCsvReader, read()).Times(0);
	EXPECT_CALL(*mockCsvWriter, append(line1)).Times(1);
	EXPECT_CALL(*mockCsvWriter, append(line2)).Times(1);
	EXPECT_CALL(*mockCsvWriter, append(line3)).Times(1);

	CsvTemperatureHistoryRepository temperatureHistoryRepository(
			std::make_shared<MockCsvReaderFactory>(mockCsvReader),
			std::make_shared<MockCsvWriterFactory>(mockCsvWriter)
		);

	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(UtcDateTime::create(2021, 6, 1, 20, 56, 3), 25));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(UtcDateTime::create(2021, 6, 1, 20, 56, 4), 30));
	temperatureHistoryRepository.add(TemperatureHistoryRepository::Sample(UtcDateTime::create(2021, 6, 1, 20, 56, 5), 35));

	temperatureHistoryRepository.save();
}

