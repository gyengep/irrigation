#include "HotWeatherSchedulerImpl.h"
#include "Logger/Logger.h"
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

HotWeatherSchedulerImpl::HotWeatherSchedulerImpl(const std::shared_ptr<TemperatureHistory>& temperatureHistory) :
	HotWeatherSchedulerImpl(temperatureHistory, std::chrono::hours(2), 35)
{
}

HotWeatherSchedulerImpl::HotWeatherSchedulerImpl(
		const std::shared_ptr<TemperatureHistory>& temperatureHistory,
		const std::chrono::seconds& period, float minTemperature
	) :
	temperatureHistory(temperatureHistory),
	lastRun(0),
	periodInSeconds(period.count()),
	minTemperature(minTemperature)
{
}

HotWeatherSchedulerImpl::~HotWeatherSchedulerImpl() {
}

void HotWeatherSchedulerImpl::setMinTemperature(float minTemperature) {
	this->minTemperature = minTemperature;
}

void HotWeatherSchedulerImpl::setPeriod(const std::chrono::seconds& period) {
	this->periodInSeconds = period.count();
}

Scheduler::Result HotWeatherSchedulerImpl::process(const std::time_t rawtime) {

	const DateTime dateTime(rawtime);

	const auto historyValues = temperatureHistory->getTemperatureHistory(dateTime.addSeconds(-periodInSeconds), dateTime);
	const bool periodIsOk = (lastRun + periodInSeconds <= rawtime);
	const bool temperatureIsOk = (historyValues.avg >= minTemperature);

	if (LOGGER.isLoggable(LogLevel::TRACE)) {
		std::ostringstream oss;
		oss << "HotWeatherSchedulerImpl::process()\n";
		oss << "\t";
		oss << "tempearture min: " << historyValues.min << ", ";
		oss << "max: " << historyValues.max << ", ";
		oss << "avg: " << historyValues.avg << "  ";
		oss << (temperatureIsOk ? "OK" : "SKIPPED") << "\n";

		oss << "\tlast run: " << ((rawtime - lastRun) / 60) << " minutes ago  " << (periodIsOk ? "OK" : "SKIPPED");
		LOGGER.trace(oss.str().c_str());
	}

	if (periodIsOk && temperatureIsOk) {
		lastRun = rawtime;
		return Scheduler::Result(true);
	}

	return Scheduler::Result(false);
}

nlohmann::json HotWeatherSchedulerImpl::saveTo() const {
	nlohmann::json j;
	j["lastRun"] = lastRun;
	return j;
}

void HotWeatherSchedulerImpl::loadFrom(const nlohmann::json& values) {
	auto it = values.find("lastRun");
	if (values.end() != it) {
		lastRun = it.value();
	}
}

HotWeatherSchedulerDTO HotWeatherSchedulerImpl::toHotWeatherSchedulerDto() const {
	return HotWeatherSchedulerDTO(periodInSeconds, minTemperature);
}

void HotWeatherSchedulerImpl::updateFromHotWeatherSchedulerDto(const HotWeatherSchedulerDTO& schedulerDTO) {
	if (schedulerDTO.hasMinTemperature()) {
		setMinTemperature(schedulerDTO.getMinTemperature());
	}

	if (schedulerDTO.hasPeriodInSeconds()) {
		setPeriod(std::chrono::seconds(schedulerDTO.getPeriodInSeconds()));
	}
}

std::string HotWeatherSchedulerImpl::toString() const {
	std::ostringstream oss;
	oss << "HotWeatherScheduler{";
	oss << "periodInSeconds=" << periodInSeconds << ", ";
	oss << "minTemperature=" << minTemperature;
	oss << "}";
	return oss.str();
}
