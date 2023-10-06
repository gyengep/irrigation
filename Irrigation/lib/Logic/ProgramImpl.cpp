#include "ProgramImpl.h"
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
	const std::shared_ptr<SchedulerContainerFactory>& schedulerContainerFactory,
	const std::shared_ptr<RunTimeContainerFactory>& runTimeContainerFactory,
	const std::shared_ptr<StartTimeContainerFactory>& startTimeContainerFactory
) :
	schedulerContainerFactory(schedulerContainerFactory),
	runTimeContainerFactory(runTimeContainerFactory),
	startTimeContainerFactory(startTimeContainerFactory)
{
}

ProgramPtr ProgramImplFactory::create(unsigned id) const {
	return std::make_shared<ProgramImpl>(
		schedulerContainerFactory->create(id),
		runTimeContainerFactory->create(),
		startTimeContainerFactory->create()
	);
}

///////////////////////////////////////////////////////////////////////////////

ProgramImpl::ProgramImpl(
	const std::shared_ptr<SchedulerContainer>& schedulerContainer,
	const std::shared_ptr<RunTimeContainer>& runTimeContainer,
	const std::shared_ptr<StartTimeContainer>& startTimeContainer
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
	const std::shared_ptr<SchedulerContainer>& schedulerContainer,
	const std::shared_ptr<RunTimeContainer>& runTimeContainer,
	const std::shared_ptr<StartTimeContainer>& startTimeContainer
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

ProgramDTO ProgramImpl::toProgramDto() const {
	return ProgramDTO(enabled, name, adjustment,
			to_string(getSchedulerType()),
			getSchedulerContainer().toSchedulersDto(),
			getRunTimeContainer().toRunTimeDtoList(),
			getStartTimeContainer().toStartTimeDtoList()
		);
}

void ProgramImpl::updateFromProgramDto(const ProgramDTO& programDTO) {
	if (programDTO.hasEnabled()) {
		setEnabled(programDTO.getEnabled());
	}

	if (programDTO.hasName()) {
		setName(programDTO.getName());
	}

	if (programDTO.hasAdjustment()) {
		setAdjustment(programDTO.getAdjustment());
	}

	if (programDTO.hasSchedulerType()) {
		const static std::string everyDaySchedulerText = to_string(SchedulerType::EVERY_DAY);
		const static std::string hotWeatherSchedulerText = to_string(SchedulerType::HOT_WEATHER);
		const static std::string temperatureDependentSchedulerText = to_string(SchedulerType::TEMPERATURE_DEPENDENT);
		const static std::string weeklySchedulerText = to_string(SchedulerType::WEEKLY);

		if (everyDaySchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::EVERY_DAY);
		} else if (hotWeatherSchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::HOT_WEATHER);
		} else if (temperatureDependentSchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT);
		} else if (weeklySchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::WEEKLY);
		} else {
			throw std::invalid_argument("ProgramImpl::updateFromDTO(): invalid SchedulerType: " + programDTO.getSchedulerType());
		}
	}

	if (programDTO.hasSchedulers()) {
		schedulerContainer->updateFromSchedulersDto(programDTO.getSchedulers());
	}

	if (programDTO.hasRunTimes()) {
		runTimeContainer->updateFromRunTimeDtoList(programDTO.getRunTimes());
	}

	if (programDTO.hasStartTimes()) {
		startTimeContainer->updateFromStartTimeDtoList(programDTO.getStartTimes());
	}
}

std::string ProgramImpl::toString() const {
	std::ostringstream oss;

	oss << "Program{";
	oss << "name=\"" << getName() << "\", ";
	oss << "enabled=" << to_string(isEnabled()) << ", ";
	oss << "adjustment=" << getAdjustment() << ", ";
	oss << "schedulerType=\"" << to_string(getSchedulerType()) << "\", ";
	oss << "schedulers=" << getSchedulerContainer().toString() << ", ";
	oss << "runTimes=" << getRunTimeContainer().toString() << ", ";
	oss << "startTimes=" << getStartTimeContainer().toString();
	oss << "}";

	return oss.str();
}
