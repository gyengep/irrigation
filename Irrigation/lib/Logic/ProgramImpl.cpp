#include "ProgramImpl.h"
#include "Logger/Logger.h"
#include "Schedulers/EveryDayScheduler.h"
#include "Schedulers/HotWeatherScheduler.h"
#include "Schedulers/TemperatureDependentScheduler.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Utils/TimeConversion.h"
#include "Utils/ToString.h"
#include <algorithm>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

ProgramImplFactory::ProgramImplFactory(
	const std::shared_ptr<SchedulerContainerFactory>& schedulerContainerFactory,
	const std::shared_ptr<RunTimeContainerFactory>& runTimeContainerFactory,
	const std::shared_ptr<StartTimeContainerFactory>& startTimeContainerFactory,
	const std::shared_ptr<StartTimeFactory>& startTimeFactory
) :
	schedulerContainerFactory(schedulerContainerFactory),
	runTimeContainerFactory(runTimeContainerFactory),
	startTimeContainerFactory(startTimeContainerFactory),
	startTimeFactory(startTimeFactory)
{
}

ProgramPtr ProgramImplFactory::create() const {
	return std::make_shared<ProgramImpl>(
		schedulerContainerFactory->create(),
		runTimeContainerFactory->create(),
		startTimeContainerFactory->create(),
		startTimeFactory
	);
}

///////////////////////////////////////////////////////////////////////////////

ProgramImpl::ProgramImpl(
	const std::shared_ptr<SchedulerContainer>& schedulerContainer,
	const std::shared_ptr<RunTimeContainer>& runTimeContainer,
	const std::shared_ptr<StartTimeContainer>& startTimeContainer,
	const std::shared_ptr<StartTimeFactory>& startTimeFactory
) : ProgramImpl(
		true, "", 100, SchedulerType::WEEKLY,
		schedulerContainer,
		runTimeContainer,
		startTimeContainer,
		startTimeFactory
	)
{
}

ProgramImpl::ProgramImpl(
	bool enabled, const std::string& name, unsigned adjustment, SchedulerType schedulerType,
	const std::shared_ptr<SchedulerContainer>& schedulerContainer,
	const std::shared_ptr<RunTimeContainer>& runTimeContainer,
	const std::shared_ptr<StartTimeContainer>& startTimeContainer,
	const std::shared_ptr<StartTimeFactory>& startTimeFactory
) :
	enabled(enabled),
	name(name),
	adjustment(adjustment),
	schedulerType(schedulerType),
	schedulerContainer(schedulerContainer),
	runTimeContainer(runTimeContainer),
	startTimeContainer(startTimeContainer),
	startTimeFactory(startTimeFactory)
{
}

Scheduler& ProgramImpl::getCurrentScheduler() {
	return *getSchedulerContainer().at(schedulerType);
}

std::unique_ptr<ScheduledResult> ProgramImpl::isScheduled(const DateTime& dateTime) {
	if (enabled) {
		for (const auto& startTimeAndIdPair : getStartTimeContainer()) {
			const StartTime& startTime = *startTimeAndIdPair.second;
			const LocalDateTime localDateTime(dateTime);

			if (startTime.equals(localDateTime.getHours(), localDateTime.getMinutes(), localDateTime.getSeconds())) {
				const Scheduler::Result result = getCurrentScheduler().process(dateTime);

				unsigned adjustment = 0;

				if (result.isScheduled) {
					adjustment = getAdjustment();

					if (result.overrideAdjustment) {
						adjustment *= (result.adjustment / 100.0f);
						LOGGER.debug("The scheduler overrides the adjustment\n"
								"\tuser adjustment        %d%%\n"
								"\tscheduler adjustment   %d%%\n"
								"\taccumulated adjustment %d%%",
								getAdjustment(),
								result.adjustment,
								adjustment
							);
					}
				}

				return std::unique_ptr<ScheduledResult>(new ScheduledResult(true, adjustment));
			}
		}
	}

	return std::unique_ptr<ScheduledResult>(new ScheduledResult(false, 0));
}

std::pair<IdType, StartTimePtr> ProgramImpl::createStartTime(const StartTimeDTO& startTimeDto) {
	StartTimePtr startTime = startTimeFactory->create();
	startTime->updateFromStartTimeDto(startTimeDto);
	return startTimeContainer->insert(IdType(), startTime);
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
		startTimeContainer->updateFromStartTimeDtoList(startTimeFactory, programDTO.getStartTimes());
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
