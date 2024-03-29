#include "Impl/SchedulerContainerImpl.h"
#include "Exceptions/Exceptions.h"
#include "Schedulers/EveryDayScheduler.h"
#include "Schedulers/HotWeatherScheduler.h"
#include "Schedulers/TemperatureDependentScheduler.h"
#include "Schedulers/WeeklyScheduler.h"
#include <sstream>


SchedulerContainerImplFactory::SchedulerContainerImplFactory(
		const EveryDaySchedulerFactoryPtr& everyDaySchedulerFactory,
		const HotWeatherSchedulerFactoryPtr& hotWeatherSchedulerFactory,
		const TemperatureDependentSchedulerFactoryPtr& temperatureDependentSchedulerFactory,
		const WeeklySchedulerFactoryPtr& weeklySchedulerFactory
) :
	everyDaySchedulerFactory(everyDaySchedulerFactory),
	hotWeatherSchedulerFactory(hotWeatherSchedulerFactory),
	temperatureDependentSchedulerFactory(temperatureDependentSchedulerFactory),
	weeklySchedulerFactory(weeklySchedulerFactory)
{
}

SchedulerContainerPtr SchedulerContainerImplFactory::create() const {
	return std::make_shared<SchedulerContainerImpl>(
			everyDaySchedulerFactory->create(),
			hotWeatherSchedulerFactory->create(),
			temperatureDependentSchedulerFactory->create(),
			weeklySchedulerFactory->create()
		);
}

///////////////////////////////////////////////////////////////////////////////

SchedulerContainerImpl::SchedulerContainerImpl(
	const EveryDaySchedulerPtr& everyDayScheduler,
	const HotWeatherSchedulerPtr& hotWeatherScheduler,
	const TemperatureDependentSchedulerPtr& temperatureDependentScheduler,
	const WeeklySchedulerPtr& weeklyScheduler
) :
	everyDayScheduler(everyDayScheduler),
	hotWeatherScheduler(hotWeatherScheduler),
	temperatureDependentScheduler(temperatureDependentScheduler),
	weeklyScheduler(weeklyScheduler)
{
	container.insert(make_pair(SchedulerType::EVERY_DAY, everyDayScheduler));
	container.insert(make_pair(SchedulerType::HOT_WEATHER, hotWeatherScheduler));
	container.insert(make_pair(SchedulerType::TEMPERATURE_DEPENDENT, temperatureDependentScheduler));
	container.insert(make_pair(SchedulerType::WEEKLY, weeklyScheduler));
}

SchedulerContainerImpl::const_mapped_type SchedulerContainerImpl::at(const key_type& key) const {
	const auto it = container.find(key);

	if (container.end() == it) {
		throw NoSuchElementException("Scheduler[" + to_string(key) + "] does not exist");
	}

	return it->second;
}

SchedulerContainerImpl::mapped_type SchedulerContainerImpl::at(const key_type& key) {
	const auto it = container.find(key);

	if (container.end() == it) {
		throw NoSuchElementException("Scheduler[" + to_string(key) + "] does not exist");
	}

	return it->second;
}

const EveryDayScheduler& SchedulerContainerImpl::getEveryDayScheduler() const {
	return *everyDayScheduler;
}

EveryDayScheduler& SchedulerContainerImpl::getEveryDayScheduler() {
	return *everyDayScheduler;
}

const HotWeatherScheduler& SchedulerContainerImpl::getHotWeatherScheduler() const {
	return *hotWeatherScheduler;
}

HotWeatherScheduler& SchedulerContainerImpl::getHotWeatherScheduler() {
	return *hotWeatherScheduler;
}

const TemperatureDependentScheduler& SchedulerContainerImpl::getTemperatureDependentScheduler() const {
	return *temperatureDependentScheduler;
}

TemperatureDependentScheduler& SchedulerContainerImpl::getTemperatureDependentScheduler() {
	return *temperatureDependentScheduler;
}

const WeeklyScheduler& SchedulerContainerImpl::getWeeklyScheduler() const {
	return *weeklyScheduler;
}

WeeklyScheduler& SchedulerContainerImpl::getWeeklyScheduler() {
	return *weeklyScheduler;
}

SchedulersDto SchedulerContainerImpl::toSchedulersDto() const {
	return SchedulersDto(
		everyDayScheduler->toEveryDaySchedulerDto(),
		hotWeatherScheduler->toHotWeatherSchedulerDto(),
		temperatureDependentScheduler->toTemperatureDependentSchedulerDto(),
		weeklyScheduler->toWeeklySchedulerDto()
	);
}

void SchedulerContainerImpl::updateFromSchedulersDto(const SchedulersDto& schedulersDto) {
	if (schedulersDto.hasEveryDayScheduler()) {
		everyDayScheduler->updateFromEveryDaySchedulerDto(schedulersDto.getEveryDayScheduler());
	}

	if (schedulersDto.hasHotWeatherScheduler()) {
		hotWeatherScheduler->updateFromHotWeatherSchedulerDto(schedulersDto.getHotWeatherScheduler());
	}

	if (schedulersDto.hasTemperatureDependentScheduler()) {
		temperatureDependentScheduler->updateFromTemperatureDependentSchedulerDto(schedulersDto.getTemperatureDependentScheduler());
	}

	if (schedulersDto.hasWeeklyScheduler()) {
		weeklyScheduler->updateFromWeeklySchedulerDto(schedulersDto.getWeeklyScheduler());
	}
}

std::string SchedulerContainerImpl::toString() const {
	std::ostringstream oss;
	oss << "[";
	for (auto it = begin(); it != end(); ++it) {
		if (it != begin()) {
			oss << ", ";
		}
		oss << "{" << to_string(it->first) << ", " << it->second->toString() << "}";
	}
	oss << "]";
	return oss.str();
}
