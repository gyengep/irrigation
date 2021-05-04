#include "TemperatureHistoryImpl.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"
#include "Utils/TimeConversion.h"
#include <iomanip>
#include <sstream>

using namespace std;


TemperatureHistoryImpl::TemperatureHistoryImpl(
	const std::shared_ptr<CurrentTemperature>& currentTemperature,
	const std::shared_ptr<TemperatureHistoryPersister>& temperatureHistoryPersister,
	const std::chrono::seconds& historyLength
) :
	currentTemperature(currentTemperature),
	temperatureHistoryPersister(temperatureHistoryPersister),
	historyLength(historyLength)
{
	if (nullptr == currentTemperature) {
		throw std::invalid_argument("TemperatureHistoryImpl::TemperatureHistoryImpl() nullptr == currentTemperature");
	}

	if (nullptr == temperatureHistoryPersister) {
		throw std::invalid_argument("TemperatureHistoryImpl::TemperatureHistoryImpl() nullptr == temperatureHistoryPersister");
	}
}

TemperatureHistoryImpl::~TemperatureHistoryImpl() {
}

void TemperatureHistoryImpl::registerToListener() {
	currentTemperature->addListener(this);
}

void TemperatureHistoryImpl::unregisterFromListener() {
	currentTemperature->removeListener(this);
}

TemperatureHistoryImpl::Values TemperatureHistoryImpl::getTemperatureHistory(const time_t& from, const time_t& to) const {
	lock_guard<mutex> lock(mtx);

	const auto samples = temperatureHistoryPersister->getBetween(from, to);

	if (samples.empty()) {
		throw TemperatureException("Temperature history not found with specified criteria");
	}

	float minValue = std::numeric_limits<float>::max();
	float maxValue = std::numeric_limits<float>::min();
	float sum = 0;

	for (const auto& sample : samples) {
		minValue = std::min(minValue, sample.temperature);
		maxValue = std::max(maxValue, sample.temperature);
		sum += sample.temperature;
	}

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		std::ostringstream oss;

		oss << "Querying temperature history: ";
		oss << toLocalTimeStr(from, "%F %T") << "-" << toLocalTimeStr(to, "%F %T") << ". ";
		oss << "Result: [" << std::fixed << std::setw(2) << std::setprecision(1) << minValue << "-";
		oss << std::fixed << std::setw(2) << std::setprecision(1) << maxValue << "]";

		LOGGER.debug(oss.str().c_str());
	}

	return Values(minValue, maxValue, sum / samples.size());
}

void TemperatureHistoryImpl::onTemperatureUpdated(const time_t& rawTime, float temperature) {
	lock_guard<mutex> lock(mtx);

	LOGGER.debug("Temperature history is updated with new value: %.1fC", temperature);

	temperatureHistoryPersister->add(TemperatureHistoryPersister::Sample(rawTime, temperature));
	temperatureHistoryPersister->removeOlder(rawTime - std::chrono::duration_cast<std::chrono::seconds>(historyLength).count());
	temperatureHistoryPersister->removeNewer(rawTime);
}
