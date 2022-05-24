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
	const std::shared_ptr<EveryDaySchedulerFactory>& everyDaySchedulerFactory,
	const std::shared_ptr<HotWeatherSchedulerFactory>& hotWeatherSchedulerFactory,
	const std::shared_ptr<TemperatureDependentSchedulerFactory>& temperatureDependentSchedulerFactory,
	const std::shared_ptr<WeeklySchedulerFactory>& weeklySchedulerFactory,
	const std::shared_ptr<RunTimeContainerFactory>& runTimeContainerFactory,
	const std::shared_ptr<StartTimeContainerFactory>& startTimeContainerFactory,
	const std::shared_ptr<StartTimeFactory>& startTimeFactory
) :
	everyDaySchedulerFactory(everyDaySchedulerFactory),
	hotWeatherSchedulerFactory(hotWeatherSchedulerFactory),
	temperatureDependentSchedulerFactory(temperatureDependentSchedulerFactory),
	weeklySchedulerFactory(weeklySchedulerFactory),
	runTimeContainerFactory(runTimeContainerFactory),
	startTimeContainerFactory(startTimeContainerFactory),
	startTimeFactory(startTimeFactory)
{
}

ProgramPtr ProgramImplFactory::create() const {
	return std::make_shared<ProgramImpl>(
		everyDaySchedulerFactory->create(),
		hotWeatherSchedulerFactory->create(),
		temperatureDependentSchedulerFactory->create(),
		weeklySchedulerFactory->create(),
		runTimeContainerFactory->create(),
		startTimeContainerFactory->create(),
		startTimeFactory
	);
}

///////////////////////////////////////////////////////////////////////////////

ProgramImpl::ProgramImpl(
	const std::shared_ptr<EveryDayScheduler>& everyDayScheduler,
	const std::shared_ptr<HotWeatherScheduler>& hotWeatherScheduler,
	const std::shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler,
	const std::shared_ptr<WeeklyScheduler>& weeklyScheduler,
	const std::shared_ptr<RunTimeContainer>& runTimeContainer,
	const std::shared_ptr<StartTimeContainer>& startTimeContainer,
	const std::shared_ptr<StartTimeFactory>& startTimeFactory
) : ProgramImpl(
		true, "", 100, SchedulerType::WEEKLY,
		everyDayScheduler,
		hotWeatherScheduler,
		temperatureDependentScheduler,
		weeklyScheduler,
		runTimeContainer,
		startTimeContainer,
		startTimeFactory
	)
{
}

ProgramImpl::ProgramImpl(
	bool enabled, const std::string& name, unsigned adjustment, SchedulerType schedulerType,
	const std::shared_ptr<EveryDayScheduler>& everyDayScheduler,
	const std::shared_ptr<HotWeatherScheduler>& hotWeatherScheduler,
	const std::shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler,
	const std::shared_ptr<WeeklyScheduler>& weeklyScheduler,
	const std::shared_ptr<RunTimeContainer>& runTimeContainer,
	const std::shared_ptr<StartTimeContainer>& startTimeContainer,
	const std::shared_ptr<StartTimeFactory>& startTimeFactory
) :
	enabled(enabled),
	name(name),
	adjustment(adjustment),
	everyDayScheduler(everyDayScheduler),
	hotWeatherScheduler(hotWeatherScheduler),
	temperatureDependentScheduler(temperatureDependentScheduler),
	weeklyScheduler(weeklyScheduler),
	runTimeContainer(runTimeContainer),
	startTimeContainer(startTimeContainer),
	startTimeFactory(startTimeFactory)
{
	setSchedulerType(schedulerType);
}

void ProgramImpl::setSchedulerType(SchedulerType schedulerType) {

	switch (schedulerType) {
	case SchedulerType::EVERY_DAY:
		currentScheduler = everyDayScheduler;
		break;
	case SchedulerType::HOT_WEATHER:
		currentScheduler = hotWeatherScheduler;
		break;
	case SchedulerType::TEMPERATURE_DEPENDENT:
		currentScheduler = temperatureDependentScheduler;
		break;
	case SchedulerType::WEEKLY:
		currentScheduler = weeklyScheduler;
		break;
	default:
		throw std::invalid_argument("ProgramImpl::setSchedulerType(): unknown SchedulerType " + std::to_string(static_cast<unsigned>(schedulerType)));
	}

	this->schedulerType = schedulerType;
}

SchedulerType ProgramImpl::getSchedulerType(void) const {
	return schedulerType;
}

std::unique_ptr<ScheduledResult> ProgramImpl::isScheduled(const std::time_t rawtime) {
	if (enabled) {
		for (const auto& startTimeAndIdPair : getStartTimeContainer()) {
			const StartTime& startTime = *startTimeAndIdPair.second;
			struct tm timeinfo = toLocalTime(rawtime);

			if (startTime.equals(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec)) {
				const Scheduler::Result result = getCurrentScheduler().process(rawtime);

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
			getEveryDayScheduler().toEveryDaySchedulerDto(),
			getHotWeatherScheduler().toHotWeatherSchedulerDto(),
			getTemperatureDependentScheduler().toTemperatureDependentSchedulerDto(),
			getWeeklyScheduler().toWeeklySchedulerDto(),
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

	if (programDTO.hasEveryDayScheduler()) {
		everyDayScheduler->updateFromEveryDaySchedulerDto(programDTO.getEveryDayScheduler());
	}

	if (programDTO.hasHotWeatherScheduler()) {
		hotWeatherScheduler->updateFromHotWeatherSchedulerDto(programDTO.getHotWeatherScheduler());
	}

	if (programDTO.hasTemperatureDependentScheduler()) {
		temperatureDependentScheduler->updateFromTemperatureDependentSchedulerDto(programDTO.getTemperatureDependentScheduler());
	}

	if (programDTO.hasWeeklyScheduler()) {
		weeklyScheduler->updateFromWeeklySchedulerDto(programDTO.getWeeklyScheduler());
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
	oss << "schedulers=[" <<
			getEveryDayScheduler().toString() << ", " <<
			getHotWeatherScheduler().toString() << ", " <<
			getTemperatureDependentScheduler().toString() << ", " <<
			getWeeklyScheduler().toString() << "], ";
	oss << "runTimes=" << getRunTimeContainer().toString() << ", ";
	oss << "startTimes=" << getStartTimeContainer().toString();
	oss << "}";

	return oss.str();
}

nlohmann::json ProgramImpl::saveTo() const {
	nlohmann::json result;
	result["temperatureDependent"] = temperatureDependentScheduler->saveTo();
	result["hotWeather"] = hotWeatherScheduler->saveTo();
	return result;
}

void ProgramImpl::loadFrom(const nlohmann::json& values) {
	auto it = values.find("temperatureDependent");
	if (values.end() != it) {
		temperatureDependentScheduler->loadFrom(it.value());
	}

	it = values.find("hotWeather");
	if (values.end() != it) {
		hotWeatherScheduler->loadFrom(it.value());
	}
}
