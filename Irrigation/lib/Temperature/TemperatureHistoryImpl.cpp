#include "TemperatureHistoryImpl.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"
#include "Utils/CsvReader.h"
#include "Utils/CsvWriter.h"
#include <algorithm>
#include <limits>
#include <fstream>

using namespace std;


TemperatureHistoryImpl::TemperatureSample::TemperatureSample(const time_t& sampleTime, float value) :
	sampleTime(sampleTime),
	value(value)
{
}

bool TemperatureHistoryImpl::TemperatureSample::operator== (const TemperatureHistoryImpl::TemperatureSample& other) const {
	return (sampleTime == other.sampleTime && value == other.value);
}

ostream& operator<<(ostream& os, const TemperatureHistoryImpl::TemperatureSample& temperatureSample) {
	os << "TemperatureSample{";
	os << "time: " << temperatureSample.sampleTime << ", ";
	os << "value: " << temperatureSample.value;
	os << "}";
	return os;
}

///////////////////////////////////////////////////////////////////////////////

TemperatureHistoryImpl::TemperatureHistoryImpl(
		const shared_ptr<CurrentTemperature>& sensor,
		const chrono::seconds& storePeriod,
		const string& fileName,
		const shared_ptr<CsvReaderFactory>& csvReaderFactory,
		const shared_ptr<CsvWriterFactory>& csvWriterFactory) :

	sensor(sensor),
	storedPeriod(storePeriod),
	fileName(fileName),
	csvReaderFactory(csvReaderFactory),
	csvWriterFactory(csvWriterFactory)
{
	load();
}

TemperatureHistoryImpl::~TemperatureHistoryImpl() {
	save();
}

void TemperatureHistoryImpl::load() {
	if (!fileName.empty()) {
		auto ifs = make_shared<ifstream>(fileName);
		if (ifs->is_open()) {
			try {
				auto csvReader = csvReaderFactory->create(ifs);
				unique_ptr<vector<string>> result;

				while ((result = csvReader->read()) != nullptr) {
					if (result->size() != 2) {
						throw exception();
					}

					addTemperature(stoul(result->at(0)), stof(result->at(1)));
				}

				LOGGER.debug("Temperature history cache file is successfully loaded. %zu items are added", samples.size());

			} catch (const exception& e) {
				LOGGER.warning("Invalid temperature sample file");
				samples.clear();
			}
		} else {
			LOGGER.debug("Temperature history cache file not found");
		}
	}
}

void TemperatureHistoryImpl::save() {
	if (!fileName.empty()) {
		auto ofs = make_shared<ofstream>(fileName);
		if (ofs->is_open()) {
			auto csvWriter = csvWriterFactory->create(ofs);

			for (const auto& sample : samples) {
				csvWriter->append(vector<string>{
					to_string(sample.sampleTime),
					temperatureToString(sample.value)
				});
			}

			LOGGER.debug("Temperature history cache file is successfully saved");
		}
	}
}

void TemperatureHistoryImpl::addTemperature(const time_t& rawTime, float temperature) {
	lock_guard<mutex> lock(mtx);
	removeOlder(rawTime - storedPeriod.count());
	removeNewer(rawTime);
	samples.push_back(TemperatureSample(rawTime, temperature));
}

void TemperatureHistoryImpl::removeNewer(const time_t& rawTime) {
	while(!samples.empty() && samples.back().sampleTime >= rawTime) {
		samples.pop_back();
	}
}

void TemperatureHistoryImpl::removeOlder(const time_t& rawTime) {
	auto predicate = [rawTime](const TemperatureSample& sample) {
		return sample.sampleTime > rawTime;
	};

	auto it = find_if(samples.begin(), samples.end(), predicate);
	samples.erase(samples.begin(), it);
}

const deque<TemperatureHistoryImpl::TemperatureSample> TemperatureHistoryImpl::getContainer() const {
	lock_guard<mutex> lock(mtx);
	return samples;
}

TemperatureHistoryImpl::Values TemperatureHistoryImpl::getTemperatureHistory(const time_t& from, const time_t& to) const {
	lock_guard<mutex> lock(mtx);

	float minValue = numeric_limits<float>::max();
	float maxValue = numeric_limits<float>::min();
	float sum = 0;
	size_t count = 0;

	for (const auto& sample : samples) {
		if (from <= sample.sampleTime && sample.sampleTime <= to) {
			minValue = min(minValue, sample.value);
			maxValue = max(maxValue, sample.value);
			sum += sample.value;
			count++;
		}
	}

	if (0 == count) {
		throw TemperatureException("Temperature history not found with specified criteria");
	}

	return Values(minValue, maxValue, sum / count);
}

void TemperatureHistoryImpl::updateCache(const time_t& rawTime) {
	try {
		addTemperature(rawTime, sensor->getCurrentTemperature());
	} catch (const exception& e) {
		LOGGER.warning("Can not read temperature for history", e);
	}
}

void TemperatureHistoryImpl::updateCache() {
	updateCache(time(nullptr));
}

void TemperatureHistoryImpl::startTimer() {
	sensor->addListener(this);
}

void TemperatureHistoryImpl::stopTimer() {
	sensor->removeListener(this);
}

void TemperatureHistoryImpl::onTimer() {
#ifdef ONTIMER_TRACE_LOG
	LOGGER.trace("TemperatureHistoryImpl::onTimer()");
#endif

	updateCache();
}

string TemperatureHistoryImpl::temperatureToString(float value) {
	char buffer[32];
	sprintf(buffer, "%.1f", value);
	return string(buffer);
}