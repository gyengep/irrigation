#include "TemperatureForecast.h"
#include "TemperatureException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

TemperatureForecast::Values::Values(float min, float max) :
	min(min),
	max(max)
{
}

bool TemperatureForecast::Values::operator== (const Values& other) const {
	return ((min == other.min) && (max == other.max));
}

ostream& operator<<(ostream& os, const TemperatureForecast::Values& values) {
	os << "Values{";
	os << "min: " << values.min << ", ";
	os << "max: " << values.max;
	os << "}";
	return os;
}

///////////////////////////////////////////////////////////////////////////////

TemperatureForecast::TemperatureForecast(const std::shared_ptr<TemperatureForecastProvider>& provider) :
		provider(provider)
{
}

TemperatureForecast::~TemperatureForecast() {
}

void TemperatureForecast::updateCache() {
	try {
		auto temporaryTemperatures = provider->getForecast();

		checkValueList(temporaryTemperatures);

		lock_guard<mutex> lock(mtx);
		temperatures = move(temporaryTemperatures);

	} catch (const exception& e) {
		LOGGER.warning("Can not query the weather forecast", e);
	}
}

TemperatureForecast::Values TemperatureForecast::getForecastValues(time_t from, time_t to) const {
	lock_guard<mutex> lock(mtx);

	float min = numeric_limits<float>::max();
	float max = numeric_limits<float>::min();
	bool found = false;

	for (const auto& valuesWithTimes : temperatures) {
		if ((from < valuesWithTimes.to) && (valuesWithTimes.from < to)) {
			min = std::min(min, valuesWithTimes.min);
			max = std::max(max, valuesWithTimes.max);
			found = true;
		}
	}

	if (false == found) {
		throw NoSuchElementException("Temperature forecast not available with specified criteria");
	}

	return Values(min, max);
}

const list<TemperatureForecastProvider::ValuesWithTimes> TemperatureForecast::getContainer() const {
	lock_guard<mutex> lock(mtx);
	return temperatures;
}

void TemperatureForecast::startTimer(const std::chrono::duration<int64_t>& period) {
	timer.reset(new Timer(this, period, Timer::ScheduleType::FIXED_DELAY));
	timer->start();
}

void TemperatureForecast::stopTimer() {
	timer->stop();
	timer.reset();
}

void TemperatureForecast::onTimer() {
	LOGGER.trace("TemperatureForecast::onTimer()");
	updateCache();
}

void TemperatureForecast::checkValueList(const list<TemperatureForecastProvider::ValuesWithTimes>& temperatures) {
	for (auto it = temperatures.begin(); temperatures.end() != it; ++it) {
		const auto& temperatureValue = *it;

		if (temperatureValue.from >= temperatureValue.to) {
			throw TemperatureException("Temperature forecast period from/to mismatch");
		}

		if (temperatures.begin() != it) {
			const auto& previousTemperatureValue = *prev(it);

			if (previousTemperatureValue.to != temperatureValue.from) {
				throw TemperatureException("Temperature forecast period from/to mismatch");
			}
		}
	}
}
