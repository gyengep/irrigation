#include "CsvTemperatureHistoryPersister.h"
#include "Logger/Logger.h"
#include <algorithm>
#include <fstream>


CsvTemperatureHistoryPersister::CsvTemperatureHistoryPersister(
	const std::shared_ptr<CsvReaderFactory>& csvReaderFactory,
	const std::shared_ptr<CsvWriterFactory>& csvWriterFactory
) :
	csvReaderFactory(csvReaderFactory),
	csvWriterFactory(csvWriterFactory)
{
	load();
}

CsvTemperatureHistoryPersister::~CsvTemperatureHistoryPersister() {
	save();
}

void CsvTemperatureHistoryPersister::load() {
	try {
		auto csvReader = csvReaderFactory->create();
		std::unique_ptr<std::vector<std::string>> result;

		while ((result = csvReader->read()) != nullptr) {
			if (result->size() == 2) {
				add(CsvTemperatureHistoryPersister::Sample(DateTime(stoul(result->at(0))), stof(result->at(1))));
			} else {
				LOGGER.warning("TemperatureHistoryPersister loaded invalid line");
			}
		}

		LOGGER.debug("Temperature history cache file is successfully loaded. %zu items are added", samples.size());

	} catch (const std::runtime_error& e) {
		LOGGER.warning("Invalid temperature history cache file");
		samples.clear();
	}
}

void CsvTemperatureHistoryPersister::save() {
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

void CsvTemperatureHistoryPersister::removeOlder(const DateTime& dateTime) {
	std::lock_guard<std::mutex> lock(mtx);

	auto removeIfTimeIsOlder = [&dateTime](const Sample& value) {
		return (dateTime > value.dateTime);
	};

	samples.remove_if(removeIfTimeIsOlder);
}

void CsvTemperatureHistoryPersister::removeNewer(const DateTime& dateTime) {
	std::lock_guard<std::mutex> lock(mtx);

	auto removeIfTimeIsNewer = [&dateTime](const Sample& value) {
		return (dateTime < value.dateTime);
	};

	samples.remove_if(removeIfTimeIsNewer);
}

void CsvTemperatureHistoryPersister::add(const Sample& sample) {
	std::lock_guard<std::mutex> lock(mtx);

	auto removeIfTimeIsSame = [&sample](const Sample& value) {
		return (sample.dateTime == value.dateTime);
	};

	samples.remove_if(removeIfTimeIsSame);
	samples.push_back(sample);
}

std::list<CsvTemperatureHistoryPersister::Sample> CsvTemperatureHistoryPersister::getAll() const {
	std::lock_guard<std::mutex> lock(mtx);
	return samples;
}

std::list<CsvTemperatureHistoryPersister::Sample> CsvTemperatureHistoryPersister::getBetween(const DateTime& from, const DateTime& to) const {
	std::lock_guard<std::mutex> lock(mtx);

	auto findIfTimeIsBetween = [&from, &to](const Sample& value) {
		return (from <= value.dateTime && value.dateTime <= to);
	};

	std::list<CsvTemperatureHistoryPersister::Sample> matches;
	std::copy_if(samples.begin(), samples.end(), std::back_inserter(matches), findIfTimeIsBetween);
	return matches;
}

std::string CsvTemperatureHistoryPersister::temperatureToString(float value) {
	char buffer[32];
	sprintf(buffer, "%.1f", value);
	return std::string(buffer);
}
