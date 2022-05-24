#include "ProgramImplBuilder.h"
#include "Logic/RunTimeContainerImpl.h"
#include "Logic/StartTimeContainerImpl.h"
#include "Schedulers/EveryDaySchedulerImpl.h"
#include "Schedulers/HotWeatherSchedulerImpl.h"
#include "Schedulers/TemperatureDependentSchedulerImpl.h"
#include "Schedulers/WeeklySchedulerImpl.h"
#include "Temperature/TemperatureHandler.h"


ProgramImpl::Builder::Builder() :
	enabled(true),
	name(),
	adjustment(100),
	schedulerType(SchedulerType::WEEKLY)
{
}

ProgramImpl::Builder& ProgramImpl::Builder::setEnabled(bool enabled) {
	this->enabled = enabled;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setName(const std::string& name) {
	this->name = name;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setAdjustment(unsigned adjustment) {
	this->adjustment = adjustment;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setSchedulerType(SchedulerType schedulerType) {
	this->schedulerType = schedulerType;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setEveryDayScheduler(const std::shared_ptr<EveryDayScheduler>& everyDayScheduler) {
	this->everyDayScheduler = everyDayScheduler;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setHotWeatherScheduler(const std::shared_ptr<HotWeatherScheduler>& hotWeatherScheduler) {
	this->hotWeatherScheduler = hotWeatherScheduler;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setTemperatureDependentScheduler(const std::shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler) {
	this->temperatureDependentScheduler = temperatureDependentScheduler;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setWeeklyScheduler(const std::shared_ptr<WeeklyScheduler>& weeklyScheduler) {
	this->weeklyScheduler = weeklyScheduler;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setRunTimeContainer(const std::shared_ptr<RunTimeContainer>& runTimeContainer) {
	this->runTimeContainer = runTimeContainer;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setStartTimeContainer(const std::shared_ptr<StartTimeContainer>& startTimeContainer) {
	this->startTimeContainer = startTimeContainer;
	return *this;
}

ProgramImpl::Builder& ProgramImpl::Builder::setStartTimeFactory(const std::shared_ptr<StartTimeFactory>& startTimeFactory) {
	this->startTimeFactory = startTimeFactory;
	return *this;
}

ProgramPtr ProgramImpl::Builder::build() {

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

	if (nullptr == runTimeContainer) {
		runTimeContainer = std::make_shared<RunTimeContainerImpl>(
				std::make_shared<RunTimeFactory>()
			);
	}

	if (nullptr == startTimeContainer) {
		startTimeContainer = std::make_shared<StartTimeContainerImpl>();
	}

	if (nullptr == startTimeFactory) {
		startTimeFactory = std::make_shared<StartTimeFactory>();
	}

	return std::make_shared<ProgramImpl>(
			enabled,
			name,
			adjustment,
			schedulerType,
			everyDayScheduler,
			hotWeatherScheduler,
			temperatureDependentScheduler,
			weeklyScheduler,
			runTimeContainer,
			startTimeContainer,
			startTimeFactory
		);
}

///////////////////////////////////////////////////////////////////////////////

ProgramImplFactory::Builder::Builder() {
}

ProgramImplFactory::Builder& ProgramImplFactory::Builder::setEveryDaySchedulerFactory(const std::shared_ptr<EveryDaySchedulerFactory>& everyDaySchedulerFactory) {
	this->everyDaySchedulerFactory = everyDaySchedulerFactory;
	return *this;
}

ProgramImplFactory::Builder& ProgramImplFactory::Builder::setHotWeatherSchedulerFactory(const std::shared_ptr<HotWeatherSchedulerFactory>& hotWeatherSchedulerFactory) {
	this->hotWeatherSchedulerFactory = hotWeatherSchedulerFactory;
	return *this;
}

ProgramImplFactory::Builder& ProgramImplFactory::Builder::setTemperatureDependentSchedulerFactory(const std::shared_ptr<TemperatureDependentSchedulerFactory>& temperatureDependentSchedulerFactory) {
	this->temperatureDependentSchedulerFactory = temperatureDependentSchedulerFactory;
	return *this;
}

ProgramImplFactory::Builder& ProgramImplFactory::Builder::setWeeklySchedulerFactory(const std::shared_ptr<WeeklySchedulerFactory>& weeklySchedulerFactory) {
	this->weeklySchedulerFactory = weeklySchedulerFactory;
	return *this;
}

ProgramImplFactory::Builder& ProgramImplFactory::Builder::setRunTimeContainerFactory(const std::shared_ptr<RunTimeContainerFactory>& runTimeContainerFactory) {
	this->runTimeContainerFactory = runTimeContainerFactory;
	return *this;
}

ProgramImplFactory::Builder& ProgramImplFactory::Builder::setStartTimeContainerFactory(const std::shared_ptr<StartTimeContainerFactory>& startTimeContainerFactory) {
	this->startTimeContainerFactory = startTimeContainerFactory;
	return *this;
}

ProgramImplFactory::Builder& ProgramImplFactory::Builder::setStartTimeFactory(const std::shared_ptr<StartTimeFactory>& startTimeFactory) {
	this->startTimeFactory = startTimeFactory;
	return *this;
}

std::shared_ptr<ProgramFactory> ProgramImplFactory::Builder::build() {
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

	if (nullptr == runTimeContainerFactory) {
		runTimeContainerFactory = std::make_shared<RunTimeContainerImplFactory>(
				std::make_shared<RunTimeFactory>()
			);
	}

	if (nullptr == startTimeContainerFactory) {
		startTimeContainerFactory = std::make_shared<StartTimeContainerImplFactory>();
	}

	if (nullptr == startTimeFactory) {
		startTimeFactory = std::make_shared<StartTimeFactory>();
	}

	return std::make_shared<ProgramImplFactory>(
			everyDaySchedulerFactory,
			hotWeatherSchedulerFactory,
			temperatureDependentSchedulerFactory,
			weeklySchedulerFactory,
			runTimeContainerFactory,
			startTimeContainerFactory,
			startTimeFactory
		);
}
