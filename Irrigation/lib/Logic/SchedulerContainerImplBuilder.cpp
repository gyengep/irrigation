#include "SchedulerContainerImplBuilder.h"
#include "SchedulerContainerImpl.h"
#include "Schedulers/EveryDaySchedulerImpl.h"
#include "Schedulers/HotWeatherSchedulerImpl.h"
#include "Schedulers/TemperatureDependentSchedulerImpl.h"
#include "Schedulers/WeeklySchedulerImpl.h"
#include "Temperature/TemperatureHandler.h"


SchedulerContainerImpl::Builder::Builder() {
}

SchedulerContainerImpl::Builder& SchedulerContainerImpl::Builder::setEveryDayScheduler(const std::shared_ptr<EveryDayScheduler>& everyDayScheduler) {
	this->everyDayScheduler = everyDayScheduler;
	return *this;
}

SchedulerContainerImpl::Builder& SchedulerContainerImpl::Builder::setHotWeatherScheduler(const std::shared_ptr<HotWeatherScheduler>& hotWeatherScheduler) {
	this->hotWeatherScheduler = hotWeatherScheduler;
	return *this;
}

SchedulerContainerImpl::Builder& SchedulerContainerImpl::Builder::setTemperatureDependentScheduler(const std::shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler) {
	this->temperatureDependentScheduler = temperatureDependentScheduler;
	return *this;
}

SchedulerContainerImpl::Builder& SchedulerContainerImpl::Builder::setWeeklyScheduler(const std::shared_ptr<WeeklyScheduler>& weeklyScheduler) {
	this->weeklyScheduler = weeklyScheduler;
	return *this;
}

SchedulerContainerPtr SchedulerContainerImpl::Builder::build() {

	if (nullptr == everyDayScheduler) {
		everyDayScheduler = std::make_shared<EveryDaySchedulerImpl>();
	}

	if (nullptr == hotWeatherScheduler) {
		hotWeatherScheduler = std::make_shared<HotWeatherSchedulerImpl>(
				TemperatureHandler::getInstance().getTemperatureHistory()
			);
	}

	if (nullptr == temperatureDependentScheduler) {
		temperatureDependentScheduler = std::make_shared<TemperatureDependentSchedulerImpl>(
				TemperatureHandler::getInstance().getTemperatureForecast(),
				TemperatureHandler::getInstance().getTemperatureHistory()
			);
	}

	if (nullptr == weeklyScheduler) {
		weeklyScheduler = std::make_shared<WeeklySchedulerImpl>();
	}

	return std::make_shared<SchedulerContainerImpl>(
			everyDayScheduler,
			hotWeatherScheduler,
			temperatureDependentScheduler,
			weeklyScheduler
		);
}

///////////////////////////////////////////////////////////////////////////////

SchedulerContainerImplFactory::Builder::Builder() {
}

SchedulerContainerImplFactory::Builder& SchedulerContainerImplFactory::Builder::setEveryDaySchedulerFactory(const std::shared_ptr<EveryDaySchedulerFactory>& everyDaySchedulerFactory) {
	this->everyDaySchedulerFactory = everyDaySchedulerFactory;
	return *this;
}

SchedulerContainerImplFactory::Builder& SchedulerContainerImplFactory::Builder::setHotWeatherSchedulerFactory(const std::shared_ptr<HotWeatherSchedulerFactory>& hotWeatherSchedulerFactory) {
	this->hotWeatherSchedulerFactory = hotWeatherSchedulerFactory;
	return *this;
}

SchedulerContainerImplFactory::Builder& SchedulerContainerImplFactory::Builder::setTemperatureDependentSchedulerFactory(const std::shared_ptr<TemperatureDependentSchedulerFactory>& temperatureDependentSchedulerFactory) {
	this->temperatureDependentSchedulerFactory = temperatureDependentSchedulerFactory;
	return *this;
}

SchedulerContainerImplFactory::Builder& SchedulerContainerImplFactory::Builder::setWeeklySchedulerFactory(const std::shared_ptr<WeeklySchedulerFactory>& weeklySchedulerFactory) {
	this->weeklySchedulerFactory = weeklySchedulerFactory;
	return *this;
}

SchedulerContainerFactoryPtr SchedulerContainerImplFactory::Builder::build() {
	if (nullptr == everyDaySchedulerFactory) {
		everyDaySchedulerFactory = std::make_shared<EveryDaySchedulerImplFactory>();
	}

	if (nullptr == hotWeatherSchedulerFactory) {
		hotWeatherSchedulerFactory = std::make_shared<HotWeatherSchedulerImplFactory>(
				TemperatureHandler::getInstance().getTemperatureHistory()
			);
	}

	if (nullptr == temperatureDependentSchedulerFactory) {
		temperatureDependentSchedulerFactory = std::make_shared<TemperatureDependentSchedulerImplFactory>(
				TemperatureHandler::getInstance().getTemperatureForecast(),
				TemperatureHandler::getInstance().getTemperatureHistory()
			);
	}

	if (nullptr == weeklySchedulerFactory) {
		weeklySchedulerFactory = std::make_shared<WeeklySchedulerImplFactory>();
	}

	return std::make_shared<SchedulerContainerImplFactory>(
			everyDaySchedulerFactory,
			hotWeatherSchedulerFactory,
			temperatureDependentSchedulerFactory,
			weeklySchedulerFactory
		);
}
