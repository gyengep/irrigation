#include "Program.h"
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "StartTime.h"
#include "StartTimeContainer.h"
#include "Logger/Logger.h"
#include "Schedulers/EveryDayScheduler.h"
#include "Schedulers/HotWeatherScheduler.h"
#include "Schedulers/TemperatureDependentScheduler.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Temperature/TemperatureHandler.h"
#include "Utils/TimeConversion.h"
#include "Utils/ToString.h"
#include <algorithm>
#include <sstream>

using namespace std;


ScheduledResult::ScheduledResult(bool scheduled, unsigned adjustment) :
	scheduled(scheduled),
	adjustment(adjustment)
{
}

bool ScheduledResult::isScheduled() const {
	return scheduled;
}

unsigned ScheduledResult::getAdjustment() const {
	return adjustment;
}

bool ScheduledResult::operator==(const ScheduledResult& other) const {
	return ((adjustment == other.adjustment) && (scheduled == other.scheduled));
}


Program::Program() :
	Program(true, "", 100, SchedulerType::WEEKLY,
		make_shared<EveryDayScheduler>(),
		make_shared<HotWeatherScheduler>(
				TemperatureHandler::getInstance().getTemperatureHistory()
				),
		make_shared<TemperatureDependentScheduler>(
				TemperatureHandler::getInstance().getTemperatureForecast(),
				TemperatureHandler::getInstance().getTemperatureHistory()
				),
		make_shared<WeeklyScheduler>(),
		make_shared<RunTimeContainer>(
				std::make_shared<RunTimeFactory>()
				),
		make_shared<StartTimeContainer>()
	)
{
}

Program::Program(bool enabled, const string& name, unsigned adjustment, SchedulerType schedulerType,
		const shared_ptr<EveryDayScheduler>& everyDayScheduler,
		const shared_ptr<HotWeatherScheduler>& hotWeatherScheduler,
		const shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler,
		const shared_ptr<WeeklyScheduler>& weeklyScheduler,
		const shared_ptr<RunTimeContainer>& runTimes,
		const shared_ptr<StartTimeContainer>& startTimes) :
	enabled(enabled),
	name(name),
	adjustment(adjustment),
	everyDayScheduler(everyDayScheduler),
	hotWeatherScheduler(hotWeatherScheduler),
	temperatureDependentScheduler(temperatureDependentScheduler),
	weeklyScheduler(weeklyScheduler),
	runTimes(runTimes),
	startTimes(startTimes)
{
	setSchedulerType(schedulerType);
}

Program::Program(const ProgramDTO& programDTO) : Program() {
	updateFromProgramDto(programDTO);
}

Program::~Program() {
}

bool Program::operator== (const Program& other) const {
	return (isEnabled() == other.isEnabled() &&
			getName() == other.getName() &&
			getAdjustment() == other.getAdjustment() &&
			getSchedulerType() == other.getSchedulerType() &&
			getEveryDayScheduler() == other.getEveryDayScheduler() &&
			getWeeklyScheduler() == other.getWeeklyScheduler() &&
			getRunTimes() == other.getRunTimes() &&
			getStartTimes() == other.getStartTimes());
}

void Program::setEnabled(bool enabled) {
	this->enabled = enabled;
}

bool Program::isEnabled() const {
	return enabled;
}

const string& Program::getName() const {
	return name;
}

void Program::setName(const string& name) {
	this->name = name;
}

void Program::setAdjustment(unsigned adjustment) {
	this->adjustment = adjustment;
}

unsigned Program::getAdjustment() const {
	return adjustment;
}

void Program::setSchedulerType(SchedulerType schedulerType) {

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
		throw invalid_argument("Program::setSchedulerType(): unknown SchedulerType " + to_string(static_cast<unsigned>(schedulerType)));
	}

	this->schedulerType = schedulerType;
}

SchedulerType Program::getSchedulerType(void) const {
	return schedulerType;
}

std::unique_ptr<ScheduledResult> Program::isScheduled(const std::time_t rawtime) {
	if (enabled) {
		for (const auto& startTimeAndIdPair : getStartTimes()) {
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

ProgramDTO Program::toProgramDto() const {
	return ProgramDTO(enabled, name, adjustment,
			to_string(getSchedulerType()),
			getEveryDayScheduler().toEveryDaySchedulerDto(),
			getHotWeatherScheduler().toHotWeatherSchedulerDto(),
			getTemperatureDependentScheduler().toTemperatureDependentSchedulerDto(),
			getWeeklyScheduler().toWeeklySchedulerDto(),
			getRunTimes().toRunTimeDtoList(),
			getStartTimes().toStartTimeDtoList());
}

void Program::updateFromProgramDto(const ProgramDTO& programDTO) {
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
		const static string everyDaySchedulerText = to_string(SchedulerType::EVERY_DAY);
		const static string hotWeatherSchedulerText = to_string(SchedulerType::HOT_WEATHER);
		const static string temperatureDependentSchedulerText = to_string(SchedulerType::TEMPERATURE_DEPENDENT);
		const static string weeklySchedulerText = to_string(SchedulerType::WEEKLY);

		if (everyDaySchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::EVERY_DAY);
		} else if (hotWeatherSchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::HOT_WEATHER);
		} else if (temperatureDependentSchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::TEMPERATURE_DEPENDENT);
		} else if (weeklySchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::WEEKLY);
		} else {
			throw invalid_argument("Program::updateFromDTO(): invalid SchedulerType: " + programDTO.getSchedulerType());
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
		runTimes->updateFromRunTimeDtoList(programDTO.getRunTimes());
	}

	if (programDTO.hasStartTimes()) {
		startTimes->updateFromStartTimeDtoList(programDTO.getStartTimes());
	}
}

string to_string(const Program& program) {
	ostringstream oss;
	oss << program;
	return oss.str();
}

ostream& operator<<(ostream& os, const Program& program) {
	os << "Program{";
	os << "name=\"" << program.getName() << "\", ";
	os << "enabled=" << to_string(program.isEnabled()) << ", ";
	os << "adjustment=\"" << program.getAdjustment() << "\", ";
	os << "schedulerType=\"" << to_string(program.getSchedulerType()) << "\", ";
	os << "schedulers=[" <<
			to_string(program.getEveryDayScheduler()) << ", " <<
			to_string(program.getHotWeatherScheduler()) << ", " <<
			to_string(program.getTemperatureDependentScheduler()) << ", " <<
			to_string(program.getWeeklyScheduler()) << "], ";
	os << "runTimes=" << to_string(program.getRunTimes()) << ", ";
	os << "startTimes=" << to_string(program.getStartTimes());
	os << "}";
	return os;
}

///////////////////////////////////////////////////////////////////////////////

Program::Builder::Builder() :
	enabled(true),
	adjustment(100),
	schedulerType(SchedulerType::WEEKLY)
{
}

Program::Builder::~Builder() {
}

Program::Builder& Program::Builder::setEnabled(bool enabled) {
	this->enabled = enabled;
	return *this;
}

Program::Builder& Program::Builder::setName(const string& name) {
	this->name = name;
	return *this;
}

Program::Builder& Program::Builder::setAdjustment(unsigned adjustment) {
	this->adjustment = adjustment;
	return *this;
}

Program::Builder& Program::Builder::setSchedulerType(SchedulerType schedulerType) {
	this->schedulerType = schedulerType;
	return *this;
}

Program::Builder& Program::Builder::setEveryDayScheduler(const shared_ptr<EveryDayScheduler>& everyDayScheduler) {
	this->everyDayScheduler = everyDayScheduler;
	return *this;
}

Program::Builder& Program::Builder::setHotWeatherScheduler(const shared_ptr<HotWeatherScheduler>& hotWeatherScheduler) {
	this->hotWeatherScheduler = hotWeatherScheduler;
	return *this;
}

Program::Builder& Program::Builder::setTemperatureDependentScheduler(const shared_ptr<TemperatureDependentScheduler>& temperatureDependentScheduler) {
	this->temperatureDependentScheduler = temperatureDependentScheduler;
	return *this;
}

Program::Builder& Program::Builder::setWeeklyScheduler(const shared_ptr<WeeklyScheduler>& weeklyScheduler) {
	this->weeklyScheduler = weeklyScheduler;
	return *this;
}

Program::Builder& Program::Builder::setRunTimeContainer(const shared_ptr<RunTimeContainer>& runTimes) {
	this->runTimes = runTimes;
	return *this;
}

Program::Builder& Program::Builder::setStartTimeContainer(const shared_ptr<StartTimeContainer>& startTimes) {
	this->startTimes = startTimes;
	return *this;
}

shared_ptr<Program> Program::Builder::build() {

	if (nullptr == everyDayScheduler) {
		everyDayScheduler = std::make_shared<EveryDayScheduler>();
	}

	if (nullptr == hotWeatherScheduler) {
		hotWeatherScheduler = make_shared<HotWeatherScheduler>(
				TemperatureHandler::getInstance().getTemperatureHistory()
			);
	}

	if (nullptr == temperatureDependentScheduler) {
		temperatureDependentScheduler = make_shared<TemperatureDependentScheduler>(
				TemperatureHandler::getInstance().getTemperatureForecast(),
				TemperatureHandler::getInstance().getTemperatureHistory()
			);
	}

	if (nullptr == weeklyScheduler) {
		weeklyScheduler = std::make_shared<WeeklyScheduler>();
	}

	if (nullptr == runTimes) {
		runTimes = std::make_shared<RunTimeContainer>(
					std::make_shared<RunTimeFactory>()
				);
	}

	if (nullptr == startTimes) {
		startTimes = std::make_shared<StartTimeContainer>();
	}

	return std::make_shared<Program>(
			enabled,
			name,
			adjustment,
			schedulerType,
			everyDayScheduler,
			hotWeatherScheduler,
			temperatureDependentScheduler,
			weeklyScheduler,
			runTimes,
			startTimes);
}

nlohmann::json Program::saveTo() const {
	nlohmann::json result;
	result["temperatureDependent"] = temperatureDependentScheduler->saveTo();
	result["hotWeather"] = hotWeatherScheduler->saveTo();
	return result;
}

void Program::loadFrom(const nlohmann::json& values) {
	auto it = values.find("temperatureDependent");
	if (values.end() != it) {
		temperatureDependentScheduler->loadFrom(it.value());
	}

	it = values.find("hotWeather");
	if (values.end() != it) {
		hotWeatherScheduler->loadFrom(it.value());
	}
}
