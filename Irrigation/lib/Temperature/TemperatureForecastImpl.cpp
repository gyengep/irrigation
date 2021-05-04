#include "TemperatureForecastImpl.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"
#include "Utils/TimeConversion.h"
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
	try {
		auto temporaryTemperatures = provider->readTemperatureForecast();
		checkValueList(temporaryTemperatures);
		setValues(std::move(temporaryTemperatures));
	} catch (const std::exception&) {
		invalidateValues();
		throw;
	}
}

void TemperatureForecastImpl::setValues(std::list<TemperatureForecastProvider::ValuesWithTimes>&& values) {
	lock_guard<mutex> lock(mtx);

	if (LOGGER.isLoggable(LogLevel::TRACE)) {

		LOGGER.trace("Temperature forecast successfully updated:");

		for (const auto& value : values) {
			std::ostringstream oss;

			oss << toLocalTimeStr(value.from, "%F %T") << "-" << toLocalTimeStr(value.to, "%F %T") << ": ";
			oss << "[" << std::fixed << std::setw(2) << std::setprecision(1) << value.min << "-";
			oss << std::fixed << std::setw(2) << std::setprecision(1) << value.max << "]";

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

TemperatureForecastImpl::Values TemperatureForecastImpl::getTemperatureForecast(const time_t& from, const time_t& to) const {
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
