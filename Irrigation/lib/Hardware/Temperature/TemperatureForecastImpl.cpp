#include "TemperatureForecastImpl.h"
#include "TemperatureException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

TemperatureForecastImpl::TemperatureForecastImpl(const shared_ptr<TemperatureForecastProvider>& provider) :
	provider(provider)
{
}

TemperatureForecastImpl::~TemperatureForecastImpl() {
}

void TemperatureForecastImpl::updateCache() {
	try {
		auto temporaryTemperatures = provider->getForecast();
		checkValueList(temporaryTemperatures);

		lock_guard<mutex> lock(mtx);
		temperatures = move(temporaryTemperatures);
	} catch (const exception& e) {
		LOGGER.warning("Weather forecast update failed", e);
	}
}

TemperatureForecastImpl::Values TemperatureForecastImpl::getForecastValues(const time_t& from, const time_t& to) const {
	lock_guard<mutex> lock(mtx);

	float minValue = numeric_limits<float>::max();
	float maxValue = numeric_limits<float>::min();
	bool found = false;

	for (const auto& valuesWithTimes : temperatures) {
		if ((from < valuesWithTimes.to) && (valuesWithTimes.from < to)) {
			minValue = min(minValue, valuesWithTimes.min);
			maxValue = max(maxValue, valuesWithTimes.max);
			found = true;
		}
	}

	if (false == found) {
		throw TemperatureException("Temperature forecast not available with specified criteria");
	}

	return Values(minValue, maxValue);
}

const list<TemperatureForecastProvider::ValuesWithTimes> TemperatureForecastImpl::getContainer() const {
	lock_guard<mutex> lock(mtx);
	return temperatures;
}

void TemperatureForecastImpl::startTimer(const chrono::seconds& period) {
	timer.reset(new Timer(period, Timer::ScheduleType::FIXED_DELAY, "TemperatureForecastImpl"));
	timer->add(this);
	timer->start();
}

void TemperatureForecastImpl::stopTimer() {
	timer->stop();
	timer.reset();
}

void TemperatureForecastImpl::onTimer() {
#ifdef ONTIMER_TRACE_LOG
	LOGGER.trace("TemperatureForecastImpl::onTimer()");
#endif

	updateCache();
}

void TemperatureForecastImpl::checkValueList(const list<TemperatureForecastProvider::ValuesWithTimes>& temperatures) {
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
