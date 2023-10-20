#include "Impl/ProgramImpl.h"
#include "Logger/Logger.h"
#include "Schedulers/EveryDayScheduler.h"
#include "Schedulers/HotWeatherScheduler.h"
#include "Schedulers/TemperatureDependentScheduler.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Utils/ToString.h"
#include <algorithm>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

ProgramImplFactory::ProgramImplFactory(
	const SchedulerContainerFactoryPtr& schedulerContainerFactory,
	const RunTimeContainerFactoryPtr& runTimeContainerFactory,
	const StartTimeContainerFactoryPtr& startTimeContainerFactory
) :
	schedulerContainerFactory(schedulerContainerFactory),
	runTimeContainerFactory(runTimeContainerFactory),
	startTimeContainerFactory(startTimeContainerFactory)
{
}

ProgramPtr ProgramImplFactory::create() const {
	return std::make_shared<ProgramImpl>(
		schedulerContainerFactory->create(),
		runTimeContainerFactory->create(),
		startTimeContainerFactory->create()
	);
}

///////////////////////////////////////////////////////////////////////////////

ProgramImpl::ProgramImpl(
	const SchedulerContainerPtr& schedulerContainer,
	const RunTimeContainerPtr& runTimeContainer,
	const StartTimeContainerPtr& startTimeContainer
) : ProgramImpl(
		true, "", 100, SchedulerType::WEEKLY,
		schedulerContainer,
		runTimeContainer,
		startTimeContainer
	)
{
}

ProgramImpl::ProgramImpl(
	bool enabled, const std::string& name, unsigned adjustment, SchedulerType schedulerType,
	const SchedulerContainerPtr& schedulerContainer,
	const RunTimeContainerPtr& runTimeContainer,
	const StartTimeContainerPtr& startTimeContainer
) :
	enabled(enabled),
	name(name),
	adjustment(adjustment),
	schedulerType(schedulerType),
	schedulerContainer(schedulerContainer),
	runTimeContainer(runTimeContainer),
	startTimeContainer(startTimeContainer)
{
}

std::unique_ptr<Scheduler::Result> ProgramImpl::isScheduled(const LocalDateTime& localDateTime) {
	if (enabled) {
		for (const auto& startTimeAndIdPair : getStartTimeContainer()) {
			const StartTime& startTime = *startTimeAndIdPair.second;

			if ((static_cast<int>(startTime.getHours()) == localDateTime.getHours()) && (static_cast<int>(startTime.getMinutes()) == localDateTime.getMinutes())) {
				return getSchedulerContainer().at(schedulerType)->process(localDateTime);
			}
		}
	}

	return std::unique_ptr<Scheduler::Result>();
}

ProgramDto ProgramImpl::toProgramDto() const {
	return ProgramDto(enabled, name, adjustment,
			to_string(getSchedulerType()),
			getSchedulerContainer().toSchedulersDto(),
			getRunTimeContainer().toRunTimeDtoList(),
			getStartTimeContainer().toStartTimeDtoList()
		);
}

void ProgramImpl::updateFromProgramDto(const ProgramDto& programDto) {
	if (programDto.hasEnabled()) {
		setEnabled(programDto.getEnabled());
	}

	if (programDto.hasName()) {
		setName(programDto.getName());
	}

	if (programDto.hasAdjustment()) {
		setAdjustment(programDto.getAdjustment());
	}

	if (programDto.hasSchedulerType()) {
		const static std::string everyDaySchedulerText = to_string(SchedulerType::EVERY_DAY);
		const static std::string hotWeatherSchedulerText = to_string(SchedulerType::HOT_WEATHER);
		const static std::string temperatureDependentSchedulerText = to_string(SchedulerType::TEMPERATURE_DEPENDENT);
		const static std::string weeklySchedulerText = to_string(SchedulerType::WEEKLY);

		if (everyDaySchedulerText == programDto.getSchedulerType()) {
			setSchedulerType(SchedulerType::EVERY_DAY);
		} else if (hotWeatherSchedulerText == programDto.getSchedulerType()) {
			setSchedulerType(SchedulerType::HOT_WEATHER);
		} else if (temperatureDependentSchedulerText == programDto.getSchedulerType()) {
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT);
		} else if (weeklySchedulerText == programDto.getSchedulerType()) {
			setSchedulerType(SchedulerType::WEEKLY);
		} else {
			throw std::invalid_argument("ProgramImpl::updateFromProgramDto(): invalid SchedulerType: " + programDto.getSchedulerType());
		}
	}

	if (programDto.hasSchedulers()) {
		schedulerContainer->updateFromSchedulersDto(programDto.getSchedulers());
	}

	if (programDto.hasRunTimes()) {
		runTimeContainer->updateFromRunTimeDtoList(programDto.getRunTimes());
	}

	if (programDto.hasStartTimes()) {
		startTimeContainer->updateFromStartTimeDtoList(programDto.getStartTimes());
	}
}

std::string ProgramImpl::toString() const {
	std::ostringstream oss;

	oss << "Program{";
	oss << "name=\"" << getName() << "\", ";
	oss << "enabled=" << to_string(isEnabled()) << ", ";
	oss << "adjustment=\"" << getAdjustment() << "\", ";
	oss << "schedulerType=\"" << to_string(getSchedulerType()) << "\", ";
	oss << "schedulers=" << getSchedulerContainer().toString() << ", ";
	oss << "runTimes=" << getRunTimeContainer().toString() << ", ";
	oss << "startTimes=" << getStartTimeContainer().toString();
	oss << "}";

	return oss.str();
}

nlohmann::json ProgramImpl::saveTo() const {
	nlohmann::json result;
	result["temperatureDependent"] = getSchedulerContainer().getTemperatureDependentScheduler().saveTo();
	result["hotWeather"] = getSchedulerContainer().getHotWeatherScheduler().saveTo();
	return result;
}

void ProgramImpl::loadFrom(const nlohmann::json& values) {
	auto it = values.find("temperatureDependent");
	if (values.end() != it) {
		getSchedulerContainer().getTemperatureDependentScheduler().loadFrom(it.value());
	}

	it = values.find("hotWeather");
	if (values.end() != it) {
		getSchedulerContainer().getHotWeatherScheduler().loadFrom(it.value());
	}
}
