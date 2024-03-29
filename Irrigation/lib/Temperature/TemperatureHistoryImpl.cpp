#include "Impl/TemperatureHistoryImpl.h"
#include "TemperatureException.h"
#include "Logger/Logger.h"
#include "Utils/ToString.h"
#include <iomanip>
#include <sstream>

using namespace std;


TemperatureHistoryImpl::TemperatureHistoryImpl(
	const std::shared_ptr<CurrentTemperature>& currentTemperature,
	const std::shared_ptr<TemperatureHistoryRepository>& temperatureHistoryRepository,
	const std::chrono::seconds& historyLength
) :
	currentTemperature(currentTemperature),
	temperatureHistoryRepository(temperatureHistoryRepository),
	historyLength(historyLength)
{
	if (nullptr == currentTemperature) {
		throw std::invalid_argument("TemperatureHistoryImpl::TemperatureHistoryImpl() nullptr == currentTemperature");
	}

	if (nullptr == temperatureHistoryRepository) {
		throw std::invalid_argument("TemperatureHistoryImpl::TemperatureHistoryImpl() nullptr == temperatureHistoryRepository");
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

TemperatureHistoryImpl::Values TemperatureHistoryImpl::getTemperatureHistory(const DateTime& from, const DateTime& to) const {
	lock_guard<mutex> lock(mtx);

	const auto samples = temperatureHistoryRepository->getBetween(from, to);

	if (samples.empty()) {
		throw TemperatureException("Temperature history not found from: " + LocalDateTime(from).toString() + " to: " + LocalDateTime(to).toString());
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
		oss << LocalDateTime(from).toString() << "-" << LocalDateTime(to).toString() << ". ";
		oss << "Result: [" << toCelsiusRange(minValue, maxValue) << "]";
		LOGGER.debug(oss.str().c_str());
	}

	return Values(minValue, maxValue, sum / samples.size());
}

TemperatureHistoryDto TemperatureHistoryImpl::toTemperatureHistoryDto(const DateTime& from, const DateTime& to, const std::string& dateTimeFormat) const {
	const auto values = getTemperatureHistory(from, to);

	return TemperatureHistoryDto(
			LocalDateTime(from).toString(dateTimeFormat),
			LocalDateTime(to).toString(dateTimeFormat),
			"\xE2\x84\x83",
			values.min,
			values.max,
			values.avg
		);
}

void TemperatureHistoryImpl::onTemperatureUpdated(const DateTime& dateTime, float temperature) {
	lock_guard<mutex> lock(mtx);

	LOGGER.debug("Temperature history is updated with new value: %s", toCelsius(temperature).c_str());

	temperatureHistoryRepository->add(TemperatureHistoryRepository::Sample(dateTime, temperature));
	temperatureHistoryRepository->removeOlder(dateTime - historyLength);
	temperatureHistoryRepository->removeNewer(dateTime);
}
