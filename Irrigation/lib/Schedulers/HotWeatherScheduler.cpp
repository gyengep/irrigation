#include "HotWeatherScheduler.h"
#include "Logger/Logger.h"
#include <sstream>

using namespace std;


HotWeatherScheduler::HotWeatherScheduler(const shared_ptr<TemperatureHistory>& temperatureHistory) :
	HotWeatherScheduler(temperatureHistory, chrono::hours(2), 35)
{
}

HotWeatherScheduler::HotWeatherScheduler(
		const std::shared_ptr<TemperatureHistory>& temperatureHistory,
		const std::chrono::seconds& period, float minTemperature
	) :
	temperatureHistory(temperatureHistory),
	lastRun(0),
	periodInSeconds(period.count()),
	minTemperature(minTemperature)
{
}

HotWeatherScheduler::~HotWeatherScheduler() {
}

void HotWeatherScheduler::setMinTemperature(float minTemperature) {
	this->minTemperature = minTemperature;
}

void HotWeatherScheduler::setPeriod(const chrono::seconds& period) {
	this->periodInSeconds = period.count();
}

Scheduler::Result HotWeatherScheduler::process(const std::time_t rawtime) {

	const auto historyValues = temperatureHistory->getTemperatureHistory(rawtime - periodInSeconds, rawtime);
	const bool periodIsOk = (lastRun + periodInSeconds <= rawtime);
	const bool temperatureIsOk = (historyValues.avg >= minTemperature);

	if (LOGGER.isLoggable(LogLevel::TRACE)) {
		ostringstream oss;
		oss << "HotWeatherScheduler::process()\n";
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

nlohmann::json HotWeatherScheduler::saveTo() const {
	nlohmann::json j;
	j["lastRun"] = lastRun;
	return j;
}

void HotWeatherScheduler::loadFrom(const nlohmann::json& values) {
	auto it = values.find("lastRun");
	if (values.end() != it) {
		lastRun = it.value();
	}
}

HotWeatherSchedulerDTO HotWeatherScheduler::toHotWeatherSchedulerDto() const {
	return HotWeatherSchedulerDTO(periodInSeconds, minTemperature);
}

void HotWeatherScheduler::updateFromHotWeatherSchedulerDto(const HotWeatherSchedulerDTO& schedulerDTO) {
	if (schedulerDTO.hasMinTemperature()) {
		setMinTemperature(schedulerDTO.getMinTemperature());
	}

	if (schedulerDTO.hasPeriodInSeconds()) {
		setPeriod(chrono::seconds(schedulerDTO.getPeriodInSeconds()));
	}
}

string to_string(const HotWeatherScheduler& scheduler) {
	ostringstream oss;
	oss << scheduler;
	return oss.str();
}

ostream& operator<<(ostream& os, const HotWeatherScheduler& scheduler) {
	os << "HotWeatherScheduler{";
	os << "periodInSeconds=" << scheduler.periodInSeconds << ", ";
	os << "minTemperature=" << scheduler.minTemperature;
	os << "}";
	return os;
}
