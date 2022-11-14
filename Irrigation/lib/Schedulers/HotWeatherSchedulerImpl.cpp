#include "HotWeatherSchedulerImpl.h"
#include "Logger/Logger.h"
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

HotWeatherSchedulerImplFactory::HotWeatherSchedulerImplFactory(const std::shared_ptr<TemperatureHistory>& temperatureHistory) :
	temperatureHistory(temperatureHistory)
{
}

HotWeatherSchedulerPtr HotWeatherSchedulerImplFactory::create() const {
	return std::make_shared<HotWeatherSchedulerImpl>(temperatureHistory);
}

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
	lastRun(DateTime::epoch()),
	period(period),
	minTemperature(minTemperature)
{
}

HotWeatherSchedulerImpl::~HotWeatherSchedulerImpl() {
}

void HotWeatherSchedulerImpl::setMinTemperature(float minTemperature) {
	this->minTemperature = minTemperature;
}

void HotWeatherSchedulerImpl::setPeriod(const std::chrono::seconds& period) {
	this->period = period;
}

float HotWeatherSchedulerImpl::getMinTemperature() const {
	return minTemperature;
}

unsigned HotWeatherSchedulerImpl::getPeriod() const {
	return period.count();
}

std::unique_ptr<Scheduler::Result> HotWeatherSchedulerImpl::process(const LocalDateTime& localDateTime) {
	const auto historyValues = temperatureHistory->getTemperatureHistory(localDateTime - period, localDateTime);
	const bool periodIsOk = ((lastRun + period) <= localDateTime);
	const bool temperatureIsOk = (historyValues.avg >= minTemperature);

	if (LOGGER.isLoggable(LogLevel::TRACE)) {
		std::ostringstream oss;
		oss << "HotWeatherSchedulerImpl::process()\n";
		oss << "\t";
		oss << "tempearture min: " << historyValues.min << ", ";
		oss << "max: " << historyValues.max << ", ";
		oss << "avg: " << historyValues.avg << "  ";
		oss << (temperatureIsOk ? "OK" : "SKIPPED") << "\n";

		oss << "\tlast run: " << std::chrono::duration_cast<std::chrono::minutes>(localDateTime - lastRun).count() << " minutes ago  " << (periodIsOk ? "OK" : "SKIPPED");
		LOGGER.trace(oss.str().c_str());
	}

	if (periodIsOk && temperatureIsOk) {
		lastRun = localDateTime;
		return std::unique_ptr<Scheduler::Result>(new Scheduler::Result(true));
	}

	return std::unique_ptr<Scheduler::Result>(new Scheduler::Result(false));
}

nlohmann::json HotWeatherSchedulerImpl::saveTo() const {
	nlohmann::json j;
	j["lastRun"] = lastRun.toRawTime();
	return j;
}

void HotWeatherSchedulerImpl::loadFrom(const nlohmann::json& values) {
	auto it = values.find("lastRun");
	if (values.end() != it) {
		lastRun = DateTime::create(it.value());
	}
}

HotWeatherSchedulerDTO HotWeatherSchedulerImpl::toHotWeatherSchedulerDto() const {
	return HotWeatherSchedulerDTO(getPeriod(), minTemperature);
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
	oss << "periodInSeconds=" << period.count() << ", ";
	oss << "minTemperature=" << minTemperature;
	oss << "}";
	return oss.str();
}
