#include "TemperatureStatistics.h"
#include "TemperatureException.h"
#include "TemperatureSensor.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Utils/CsvReader.h"
#include "Utils/CsvWriter.h"
#include <algorithm>
#include <limits>
#include <fstream>

using namespace std;


TemperatureStatistics::TemperatureSample::TemperatureSample(time_t sampleTime) :
	sampleTime(sampleTime),
	value(0.0f),
	valid(false)
{
}

TemperatureStatistics::TemperatureSample::TemperatureSample(time_t sampleTime, float value) :
	sampleTime(sampleTime),
	value(value),
	valid(true)
{
}

bool TemperatureStatistics::TemperatureSample::operator== (const TemperatureStatistics::TemperatureSample& other) const {
	return (sampleTime == other.sampleTime && value == other.value && valid == other.valid);
}

ostream& operator<<(ostream& os, const TemperatureStatistics::TemperatureSample& temperatureSample) {
	os << "TemperatureSample{";
	os << "time: " << temperatureSample.sampleTime << ", ";
	os << "value: " << temperatureSample.value << ", ";
	os << "valid: " << temperatureSample.valid;
	os << "}";
	return os;
}

TemperatureStatistics::GreaterThan::GreaterThan(time_t rawTime) :
	rawTime(rawTime)
{
}

bool TemperatureStatistics::GreaterThan::operator() (const TemperatureSample& sample) {
	return sample.sampleTime > rawTime;
}

///////////////////////////////////////////////////////////////////////////////

TemperatureStatistics::TemperatureStatistics(const chrono::duration<int64_t>& storePeriod) :
	TemperatureStatistics(storePeriod, string(), shared_ptr<CsvReaderFactory>(), shared_ptr<CsvWriterFactory>())
{
}

TemperatureStatistics::TemperatureStatistics(const chrono::duration<int64_t>& storePeriod,
		const string& fileName,
		const shared_ptr<CsvReaderFactory>& csvReaderFactory,
		const shared_ptr<CsvWriterFactory>& csvWriterFactory) :

	fileName(fileName),
	csvReaderFactory(csvReaderFactory),
	csvWriterFactory(csvWriterFactory),
	storedSeconds(chrono::duration_cast<chrono::seconds>(storePeriod).count())
{
	load();
}

TemperatureStatistics::~TemperatureStatistics() {
	save();
}

void TemperatureStatistics::load() {
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

				LOGGER.debug("Temperature sample file is successfully loaded. %zu items are added", samples.size());

			} catch (const exception& e) {
				LOGGER.warning("Invalid temperature sample file");
				samples.clear();
			}
		} else {
			LOGGER.debug("Temperature sample file not found");
		}
	}
}

string TemperatureStatistics::temperatureToString(float value) {
	char buffer[32];
	sprintf(buffer, "%.1f", value);
	return string(buffer);
}

void TemperatureStatistics::save() {
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

			LOGGER.debug("Temperature sample file is successfully saved");
		}
	}
}

void TemperatureStatistics::addTemperature(time_t rawTime, float temperature) {
	lock_guard<mutex> lock(mtx);
	removeOlder(rawTime - storedSeconds);
	removeNewer(rawTime);
	samples.push_back(TemperatureSample(rawTime, temperature));
}

void TemperatureStatistics::removeNewer(time_t rawTime) {
	while(!samples.empty() && samples.back().sampleTime >= rawTime) {
		samples.pop_back();
	}
}

void TemperatureStatistics::removeOlder(time_t rawTime) {
	auto it = find_if(samples.begin(), samples.end(), GreaterThan(rawTime));
	samples.erase(samples.begin(), it);
}

const deque<TemperatureStatistics::TemperatureSample> TemperatureStatistics::getSamples() const {
	lock_guard<mutex> lock(mtx);
	return samples;
}

StatisticsValues TemperatureStatistics::getStatistics(time_t from, time_t to) {
	lock_guard<mutex> lock(mtx);

	float min = numeric_limits<float>::max();
	float max = numeric_limits<float>::min();
	float sum = 0;
	size_t count = 0;

	for (const auto& sample : samples) {
		if (sample.valid && from <= sample.sampleTime && sample.sampleTime <= to) {
			min = std::min(min, sample.value);
			max = std::max(max, sample.value);
			sum += sample.value;
			count++;
		}
	}

	if (0 == count) {
		throw NoSuchElementException("Temperature sample not found found with specified criteria");
	}

	StatisticsValues statisticsValues;
	statisticsValues.minTemperature = min;
	statisticsValues.maxTemperature = max;
	statisticsValues.avgTemperature = sum / count;
	return statisticsValues;
}
