#include "HotWeatherSchedulerImpl.h"
#include "Logger/Logger.h"
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

HotWeatherSchedulerImplFactory::HotWeatherSchedulerImplFactory(
		const std::shared_ptr<HotWeatherSchedulerRepository>& repository,
		const std::shared_ptr<TemperatureHistory>& temperatureHistory
) :
	repository(repository),
	temperatureHistory(temperatureHistory)
{
}

HotWeatherSchedulerPtr HotWeatherSchedulerImplFactory::create(unsigned id) const {
	return std::make_shared<HotWeatherSchedulerImpl>(
			std::make_shared<HotWeatherSchedulerImpl::PersistedData>(repository, id),
			temperatureHistory
		);
}

///////////////////////////////////////////////////////////////////////////////

HotWeatherSchedulerImpl::PersistedData::PersistedData(
	const std::shared_ptr<HotWeatherSchedulerRepository>& repository,
	unsigned id
) :
	repository(repository),
	id(id),
	lastRun(DateTime::epoch())
{
}

void HotWeatherSchedulerImpl::PersistedData::init() {
/*
	if (false == create) {
		if (nullptr == repository->findById(id)) {
			LOGGER.warning("Program[%u].HotWeatherSchedulerEntity does not exist", id);
			create = true;
		}
	}

	if (create) {
		try {
			auto entity = std::make_shared<HotWeatherSchedulerEntity>(id);
			repository->insert(entity);
			LOGGER.debug("Program[%u].HotWeatherSchedulerEntity is added: %s", id, entity->toString().c_str());
		} catch (const std::runtime_error& e) {
			LOGGER.warning("Can not create HotWeatherSchedulerEntity", e);
		}
	}
*/
}

void HotWeatherSchedulerImpl::PersistedData::uninit() {
/*	
	try {
	repository->erase(std::make_shared<HotWeatherSchedulerEntity>(id));
		LOGGER.debug("Program[%u].HotWeatherSchedulerEntity is deleted", id);
	} catch (const std::runtime_error& e) {
		LOGGER.warning("Can not delete HotWeatherSchedulerEntity", e);
	}
*/
}

void HotWeatherSchedulerImpl::PersistedData::save(const DateTime& lastRun) {
/*
	try {
		const auto entity = std::make_shared<HotWeatherSchedulerEntity>(id, lastRun.toRawTime());
		repository->update(entity);
		LOGGER.debug("Program[%u].HotWeatherSchedulerEntity is modified: %s", id, entity->toString().c_str());
	} catch (const std::runtime_error& e) {
		LOGGER.warning("Can not update HotWeatherSchedulerEntity", e);
	}
*/
}

void HotWeatherSchedulerImpl::PersistedData::load(DateTime& lastRun) {
/*
	const auto entity = repository->findById(id);

	if (nullptr == entity) {
		throw std::runtime_error("HotWeatherSchedulerEntity[" + std::to_string(id) + "] not found");
	}

	lastRun = DateTime::create(entity->getLastRun());
*/
}

///////////////////////////////////////////////////////////////////////////////

HotWeatherSchedulerImpl::HotWeatherSchedulerImpl(
		const std::shared_ptr<PersistedData>& persistedData,
		const std::shared_ptr<TemperatureHistory>& temperatureHistory
) :
	HotWeatherSchedulerImpl(persistedData, temperatureHistory, std::chrono::hours(2), 35)
{
}

HotWeatherSchedulerImpl::HotWeatherSchedulerImpl(
		const std::shared_ptr<PersistedData>& persistedData,
		const std::shared_ptr<TemperatureHistory>& temperatureHistory,
		const std::chrono::seconds& period, float minTemperature
) :
	persistedData(persistedData),
	temperatureHistory(temperatureHistory),
	lastRun(DateTime::epoch()),
	period(period),
	minTemperature(minTemperature)
{
}

HotWeatherSchedulerImpl::~HotWeatherSchedulerImpl() {
}

void HotWeatherSchedulerImpl::createPersistedData() {
/*
	persistedData->init();
*/
}

void HotWeatherSchedulerImpl::deletePersistedData() {
/*
	try {
		persistedData->uninit();
		LOGGER.debug("Program[%u].HotWeatherSchedulerEntity is deleted", id);
	} catch (const std::runtime_error& e) {
		LOGGER.warning("Can not delete HotWeatherSchedulerEntity", e);
	}
*/
}

void HotWeatherSchedulerImpl::loadPersistedData() {
//	persistedData->load(lastRun);
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
		persistedData->save(lastRun);
		return std::unique_ptr<Scheduler::Result>(new Scheduler::Result(true));
	}

	return std::unique_ptr<Scheduler::Result>(new Scheduler::Result(false));
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
