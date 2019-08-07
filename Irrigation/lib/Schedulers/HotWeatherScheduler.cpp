#include "HotWeatherScheduler.h"
#include "Logger/Logger.h"

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

	if (lastRun + periodInSeconds <= rawtime) {
		if (temperatureHistory->getHistoryValues(rawtime - periodInSeconds, rawtime).avg >= minTemperature) {
			lastRun = rawtime;
			return Scheduler::Result(true);
		}
	}

	return Scheduler::Result(false);
}

nlohmann::json HotWeatherScheduler::saveTo() const {
	LOGGER.trace("HotWeatherScheduler::saveTo()");

	nlohmann::json j;
	j["lastRun"] = lastRun;

	LOGGER.trace("lastRun: %llu", (long long unsigned)lastRun);
	return j;
}

void HotWeatherScheduler::loadFrom(const nlohmann::json& values) {
	LOGGER.trace("HotWeatherScheduler::loadFrom()");

	{
		auto it = values.find("lastRun");
		if (values.end() != it) {
			lastRun = it.value();
		}
	}

	LOGGER.trace("lastRun: %llu", (long long unsigned)lastRun);
}
