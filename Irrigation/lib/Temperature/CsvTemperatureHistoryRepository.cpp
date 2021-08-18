#include "CsvTemperatureHistoryRepository.h"
#include "Logger/Logger.h"
#include <algorithm>
#include <fstream>


CsvTemperatureHistoryRepository::CsvTemperatureHistoryRepository(
	const std::shared_ptr<CsvReaderFactory>& csvReaderFactory,
	const std::shared_ptr<CsvWriterFactory>& csvWriterFactory
) :
	csvReaderFactory(csvReaderFactory),
	csvWriterFactory(csvWriterFactory)
{
}

CsvTemperatureHistoryRepository::~CsvTemperatureHistoryRepository() {
}

void CsvTemperatureHistoryRepository::load() {
	std::lock_guard<std::mutex> lock(mtx);
	samples.clear();

	try {
		auto csvReader = csvReaderFactory->create();
		std::unique_ptr<std::vector<std::string>> result;

		while ((result = csvReader->read()) != nullptr) {
			if (result->size() == 2) {
				add(CsvTemperatureHistoryRepository::Sample(DateTime(stoul(result->at(0))), stof(result->at(1))));
			} else {
				LOGGER.warning("TemperatureHistoryRepository loaded invalid line");
			}
		}

		LOGGER.debug("Temperature history cache file is successfully loaded. %zu items are added", samples.size());

	} catch (const std::runtime_error& e) {
		LOGGER.warning("Invalid temperature history cache file");
	}
}

void CsvTemperatureHistoryRepository::save() {
	std::lock_guard<std::mutex> lock(mtx);

	try {
		auto csvWriter = csvWriterFactory->create();

		for (const auto& sample : samples) {
			csvWriter->append(std::vector<std::string>{
				std::to_string(sample.dateTime.toRawtime()),
				temperatureToString(sample.temperature)
			});
		}

		LOGGER.debug("Temperature history cache file is successfully saved");
	} catch (const std::runtime_error& e) {
		LOGGER.warning("Can not save temperature history cache file");
	}
}

void CsvTemperatureHistoryRepository::removeOlder(const DateTime& dateTime) {
	std::lock_guard<std::mutex> lock(mtx);

	auto removeIfTimeIsOlder = [&dateTime](const Sample& value) {
		return (dateTime > value.dateTime);
	};

	samples.remove_if(removeIfTimeIsOlder);
}

void CsvTemperatureHistoryRepository::removeNewer(const DateTime& dateTime) {
	std::lock_guard<std::mutex> lock(mtx);

	auto removeIfTimeIsNewer = [&dateTime](const Sample& value) {
		return (dateTime < value.dateTime);
	};

	samples.remove_if(removeIfTimeIsNewer);
}

void CsvTemperatureHistoryRepository::add(const Sample& sample) {
	std::lock_guard<std::mutex> lock(mtx);

	auto removeIfTimeIsSame = [&sample](const Sample& value) {
		return (sample.dateTime == value.dateTime);
	};

	samples.remove_if(removeIfTimeIsSame);
	samples.push_back(sample);
}

std::list<CsvTemperatureHistoryRepository::Sample> CsvTemperatureHistoryRepository::getAll() const {
	std::lock_guard<std::mutex> lock(mtx);
	return samples;
}

std::list<CsvTemperatureHistoryRepository::Sample> CsvTemperatureHistoryRepository::getBetween(const DateTime& from, const DateTime& to) const {
	std::lock_guard<std::mutex> lock(mtx);

	auto findIfTimeIsBetween = [&from, &to](const Sample& value) {
		return (from <= value.dateTime && value.dateTime <= to);
	};

	std::list<CsvTemperatureHistoryRepository::Sample> matches;
	std::copy_if(samples.begin(), samples.end(), std::back_inserter(matches), findIfTimeIsBetween);
	return matches;
}

std::string CsvTemperatureHistoryRepository::temperatureToString(float value) {
	char buffer[32];
	sprintf(buffer, "%.1f", value);
	return std::string(buffer);
}
