#include "TemperatureForecastImpl.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"
#include "Utils/FixedDelaySchedulerRunnable.h"
#include "Utils/FunctionRunnable.h"
#include "Utils/RepeatUntilSuccessRunnable.h"
#include "Utils/TimeConversion.h"
#include "Utils/ToString.h"
#include <iomanip>
#include <sstream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

TemperatureForecastImpl::TemperatureForecastImpl(const shared_ptr<TemperatureForecastProvider>& provider) :
	provider(provider),
	values(),
	valid(false)
{
	if (nullptr == provider) {
		throw std::invalid_argument("TemperatureForecastImpl::TemperatureForecastImpl() nullptr == provider");
	}

	LOGGER.debug("%s forecast provider is initialized", provider->getForecastProviderName().c_str());
}

TemperatureForecastImpl::~TemperatureForecastImpl() {
}

void TemperatureForecastImpl::updateCache() {
	LOGGER.trace("TemperatureForecastImpl::updateCache() started");

	try {
		auto temporaryTemperatures = provider->readTemperatureForecast();
		checkValueList(temporaryTemperatures);
		setValues(std::move(temporaryTemperatures));
	} catch (const std::exception&) {
		invalidateValues();

		LOGGER.trace("TemperatureForecastImpl::updateCache() failed");
		throw;
	}

	LOGGER.trace("TemperatureForecastImpl::updateCache() finished");
}

void TemperatureForecastImpl::start(const std::chrono::milliseconds& updatePeriod, const std::vector<std::chrono::milliseconds>& delayOnFailed) {
	std::chrono::milliseconds firstDelay;

	try {
		updateCache();
		firstDelay = updatePeriod;
	} catch (const std::exception& e) {
		LOGGER.warning("Temperature forecast update failed", e);
		firstDelay = std::chrono::milliseconds(100);
	}

	auto func = [this] {
		updateCache();
	};

	auto functionRunnbale = std::make_shared<FunctionRunnable>(func);
	auto repeatUntilSuccessRunnable = std::make_shared<RepeatUntilSuccessRunnable>(
			functionRunnbale,
			delayOnFailed,
			"Temperature forecast update"
		);

	auto fixedDelaySchedulerRunnable = std::make_shared<FixedDelaySchedulerRunnable>(
			repeatUntilSuccessRunnable,
			firstDelay,
			updatePeriod,
			"TemperatureForecastImpl"
		);

	workerThread = std::unique_ptr<Thread>(new Thread(
			fixedDelaySchedulerRunnable,
			"TemperatureForecastImpl"
		));

	workerThread->start();
}

void TemperatureForecastImpl::stop() {
	workerThread->stop();
	workerThread.reset();
}

void TemperatureForecastImpl::setValues(std::list<TemperatureForecastProvider::ValuesWithTimes>&& values) {
	lock_guard<mutex> lock(mtx);

	if (LOGGER.isLoggable(LogLevel::TRACE)) {

		LOGGER.trace("Temperature forecast successfully updated:");

		for (const auto& value : values) {
			std::ostringstream oss;
			oss << LocalDateTime(value.from).toString() << "-" << LocalDateTime(value.to).toString() << ": ";
			oss << "[" << toCelsiusRange(value.min, value.max) << "]";
			LOGGER.trace(oss.str().c_str());
		}
	} else {
		LOGGER.debug("Temperature forecast successfully updated");
	}

	this->values = move(values);
	this->valid = true;
}

void TemperatureForecastImpl::invalidateValues() {
	lock_guard<mutex> lock(mtx);
	values.clear();
	valid = false;
}

TemperatureForecastImpl::Values TemperatureForecastImpl::getTemperatureForecast(const DateTime& from, const DateTime& to) const {
	lock_guard<mutex> lock(mtx);

	if (false == valid) {
		throw TemperatureException("Temperature forecast is not available");
	}

	float minValue = std::numeric_limits<float>::max();
	float maxValue = std::numeric_limits<float>::min();
	bool found = false;

	for (const auto& valuesWithTimes : values) {
		if ((from < valuesWithTimes.to) && (valuesWithTimes.from < to)) {
			minValue = std::min(minValue, valuesWithTimes.min);
			maxValue = std::max(maxValue, valuesWithTimes.max);
			found = true;
		}
	}

	if (false == found) {
		throw TemperatureException("Temperature forecast not available with specified criteria");
	}

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		std::ostringstream oss;
		oss << "Querying temperature forecast: ";
		oss << LocalDateTime(from).toString()  << "-" << LocalDateTime(to).toString() << ". ";
		oss << "Result: [" << toCelsiusRange(minValue, maxValue) << "]";
		LOGGER.debug(oss.str().c_str());
	}

	return Values(minValue, maxValue);
}

const list<TemperatureForecastProvider::ValuesWithTimes> TemperatureForecastImpl::getContainer() const {
	lock_guard<mutex> lock(mtx);
	return values;
}

void TemperatureForecastImpl::checkValueList(const list<TemperatureForecastProvider::ValuesWithTimes>& values) {
	for (auto it = values.begin(); values.end() != it; ++it) {
		const auto& temperatureValue = *it;

		if (temperatureValue.from >= temperatureValue.to) {
			throw TemperatureException("Temperature forecast period from/to mismatch");
		}

		if (values.begin() != it) {
			const auto& previousTemperatureValue = *prev(it);

			if (previousTemperatureValue.to != temperatureValue.from) {
				throw TemperatureException("Temperature forecast period from/to mismatch");
			}
		}
	}
}
