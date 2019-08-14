#include "HotWeatherScheduler.h"
#include "Logger/Logger.h"
#include <sstream>

using namespace std;


HotWeatherScheduler::HotWeatherScheduler(const shared_ptr<TemperatureHistory>& temperatureHistory) :
	temperatureHistory(temperatureHistory),
	lastRun(0),
	periodInSeconds(60 * 60 *2),
	minTemperature(33.0f)
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

	const auto historyValues = temperatureHistory->getHistoryValues(rawtime - periodInSeconds, rawtime);
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
