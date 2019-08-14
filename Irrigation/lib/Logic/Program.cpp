#include "Program.h"
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "StartTime.h"
#include "StartTimeContainer.h"
#include "Logger/Logger.h"
#include "Schedulers/EveryDayScheduler.h"
#include "Schedulers/FixedAmountScheduler.h"
#include "Schedulers/FixedPeriodScheduler.h"
#include "Schedulers/HotWeatherScheduler.h"
#include "Schedulers/PeriodicScheduler.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Hardware/Temperature/Temperature.h"
#include "Utils/ToString.h"
#include <algorithm>
#include <sstream>

using namespace std;


Program::Program() :
	Program(false, "", 100, SchedulerType::WEEKLY,
		make_shared<PeriodicScheduler>(),
		make_shared<WeeklyScheduler>(),
		make_shared<EveryDayScheduler>(),
		make_shared<RunTimeContainer>(),
		make_shared<StartTimeContainer>()
	)
{
}

Program::Program(const Program& other) :
	Program(other.isDisabled(), other.getName(),
		other.getAdjustment(),
		other.getSchedulerType(),
		shared_ptr<PeriodicScheduler>(new PeriodicScheduler(other.getPeriodicScheduler())),
		shared_ptr<WeeklyScheduler>(new WeeklyScheduler(other.getWeeklyScheduler())),
		shared_ptr<EveryDayScheduler>(new EveryDayScheduler(other.getEveryDayScheduler())),
		shared_ptr<RunTimeContainer>(new RunTimeContainer(other.getRunTimes())),
		shared_ptr<StartTimeContainer>(new StartTimeContainer(other.getStartTimes()))
	)
{
}

Program::Program(bool disabled, const string& name, unsigned adjustment, SchedulerType schedulerType,
	shared_ptr<PeriodicScheduler> periodicScheduler,
	shared_ptr<WeeklyScheduler> weeklyScheduler,
	shared_ptr<EveryDayScheduler> everyDayScheduler,
	shared_ptr<RunTimeContainer> runTimes,
	shared_ptr<StartTimeContainer> startTimes) :
	disabled(disabled),
	name(name),
	adjustment(adjustment),
	periodicScheduler(periodicScheduler),
	weeklyScheduler(weeklyScheduler),
	everyDayScheduler(everyDayScheduler),
	runTimes(runTimes),
	startTimes(startTimes)
{
	setSchedulerType(schedulerType);
}

Program::Program(bool disabled, const string& name, unsigned adjustment, SchedulerType schedulerType,
	shared_ptr<PeriodicScheduler> periodicScheduler,
	shared_ptr<WeeklyScheduler> weeklyScheduler,
	shared_ptr<EveryDayScheduler> everyDayScheduler,
	shared_ptr<FixedAmountScheduler> fixedAmountScheduler,
	shared_ptr<FixedPeriodScheduler> fixedPeriodScheduler,
	shared_ptr<HotWeatherScheduler> hotWeatherScheduler,
	shared_ptr<RunTimeContainer> runTimes,
	shared_ptr<StartTimeContainer> startTimes) :
	disabled(disabled),
	name(name),
	adjustment(adjustment),
	periodicScheduler(periodicScheduler),
	weeklyScheduler(weeklyScheduler),
	everyDayScheduler(everyDayScheduler),
	fixedAmountScheduler(fixedAmountScheduler),
	fixedPeriodScheduler(fixedPeriodScheduler),
	hotWeatherScheduler(hotWeatherScheduler),
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
	return (isDisabled() == other.isDisabled() &&
			getName() == other.getName() &&
			getAdjustment() == other.getAdjustment() &&
			getSchedulerType() == other.getSchedulerType() &&
			getPeriodicScheduler() == other.getPeriodicScheduler() &&
			getWeeklyScheduler() == other.getWeeklyScheduler() &&
			getEveryDayScheduler() == other.getEveryDayScheduler() &&
			getRunTimes() == other.getRunTimes() &&
			getStartTimes() == other.getStartTimes());
}

void Program::setDisabled(bool disabled) {
	this->disabled = disabled;
}

bool Program::isDisabled() const {
	return disabled;
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
	case SchedulerType::WEEKLY:
		currentScheduler = weeklyScheduler;
		break;
	case SchedulerType::PERIODIC:
		currentScheduler = periodicScheduler;
		break;
	case SchedulerType::EVERY_DAY:
		currentScheduler = everyDayScheduler;
		break;
	case SchedulerType::FIXED_AMOUNT:
		currentScheduler = fixedAmountScheduler;
		break;
	case SchedulerType::FIXED_PERIOD:
		currentScheduler = fixedPeriodScheduler;
		break;
	case SchedulerType::HOT_WEATHER:
		currentScheduler = hotWeatherScheduler;
		break;
	default:
		throw invalid_argument("Program::setSchedulerType(): unknown SchedulerType " + to_string(static_cast<unsigned>(schedulerType)));
	}

	this->schedulerType = schedulerType;
}

SchedulerType Program::getSchedulerType(void) const {
	return schedulerType;
}

pair<bool, unsigned> Program::isScheduled(const std::time_t rawtime) {
	if (!disabled) {
		for (const auto& startTimeAndIdPair : getStartTimes()) {
			const StartTime& startTime = *startTimeAndIdPair.second;
			struct tm timeinfo;
			localtime_r(&rawtime, &timeinfo);

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

				return make_pair(true, adjustment);
			}
		}
	}

	return make_pair(false, 0);
}

ProgramDTO Program::toProgramDto() const {
	return ProgramDTO(disabled, name, adjustment,
			to_string(getSchedulerType()),
			getPeriodicScheduler().toPeriodicSchedulerDto(),
			getWeeklyScheduler().toWeeklySchedulerDto(),
			getEveryDayScheduler().toEveryDaySchedulerDto(),
			getRunTimes().toRunTimeDtoList(),
			getStartTimes().toStartTimeDtoList());
}

void Program::updateFromProgramDto(const ProgramDTO& programDTO) {
	if (programDTO.hasDisabled()) {
		setDisabled(programDTO.getDisabled());
	}

	if (programDTO.hasName()) {
		setName(programDTO.getName());
	}

	if (programDTO.hasAdjustment()) {
		setAdjustment(programDTO.getAdjustment());
	}

	if (programDTO.hasSchedulerType()) {
		const static string periodicSchedulerText = to_string(SchedulerType::PERIODIC);
		const static string weeklySchedulerText = to_string(SchedulerType::WEEKLY);
		const static string everyDaySchedulerText = to_string(SchedulerType::EVERY_DAY);
		const static string fixedAmountSchedulerText = to_string(SchedulerType::FIXED_AMOUNT);
		const static string fixedPeriodSchedulerText = to_string(SchedulerType::FIXED_PERIOD);
		const static string hotWeatherSchedulerText = to_string(SchedulerType::HOT_WEATHER);

		if (periodicSchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::PERIODIC);
		} else if (weeklySchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::WEEKLY);
		} else if (everyDaySchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::EVERY_DAY);
		} else if (fixedAmountSchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::FIXED_AMOUNT);
		} else if (fixedPeriodSchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::FIXED_PERIOD);
		} else if (hotWeatherSchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::HOT_WEATHER);
		} else {
			throw invalid_argument("Program::updateFromDTO(): invalid SchedulerType: " + programDTO.getSchedulerType());
		}
	}

	if (programDTO.hasPeriodicScheduler()) {
		periodicScheduler->updateFromPeriodicSchedulerDto(programDTO.getPeriodicScheduler());
	}

	if (programDTO.hasWeeklyScheduler()) {
		weeklyScheduler->updateFromWeeklySchedulerDto(programDTO.getWeeklyScheduler());
	}

	if (programDTO.hasEveryDayScheduler()) {
		everyDayScheduler->updateFromEveryDaySchedulerDto(programDTO.getEveryDayScheduler());
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
	os << "disabled=" << to_string(program.isDisabled()) << ", ";
	os << "adjustment=\"" << program.getAdjustment() << "\", ";
	os << "schedulerType=\"" << to_string(program.getSchedulerType()) << "\", ";
	os << "schedulers=[" <<
			to_string(program.getPeriodicScheduler()) << ", " <<
			to_string(program.getWeeklyScheduler()) << ", " <<
			to_string(program.getEveryDayScheduler()) << ", " <<
			to_string(program.getFixedAmountScheduler()) << ", " <<
			to_string(program.getFixedPeriodScheduler()) << ", " <<
			to_string(program.getHotWeatherScheduler()) << "], ";
	os << "runTimes=" << to_string(program.getRunTimes()) << ", ";
	os << "startTimes=" << to_string(program.getStartTimes());
	os << "}";
	return os;
}

///////////////////////////////////////////////////////////////////////////////

Program::Builder::Builder() :
	disabled(false),
	adjustment(100),
	schedulerType(SchedulerType::WEEKLY)
{
}

Program::Builder::~Builder() {
}

Program::Builder& Program::Builder::setDisabled(bool disabled) {
	this->disabled = disabled;
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

Program::Builder& Program::Builder::setPeriodicScheduler(shared_ptr<PeriodicScheduler> periodicScheduler) {
	this->periodicScheduler = periodicScheduler;
	return *this;
}

Program::Builder& Program::Builder::setWeeklyScheduler(shared_ptr<WeeklyScheduler> weeklyScheduler) {
	this->weeklyScheduler = weeklyScheduler;
	return *this;
}

Program::Builder& Program::Builder::setEveryDayScheduler(shared_ptr<EveryDayScheduler> everyDayScheduler) {
	this->everyDayScheduler = everyDayScheduler;
	return *this;
}

Program::Builder& Program::Builder::setRunTimeContainer(shared_ptr<RunTimeContainer> runTimes) {
	this->runTimes = runTimes;
	return *this;
}

Program::Builder& Program::Builder::setStartTimeContainer(shared_ptr<StartTimeContainer> startTimes) {
	this->startTimes = startTimes;
	return *this;
}

shared_ptr<Program> Program::Builder::build() {

	if (nullptr == periodicScheduler) {
		periodicScheduler.reset(new PeriodicScheduler());
	}

	if (nullptr == weeklyScheduler) {
		weeklyScheduler.reset(new WeeklyScheduler());
	}

	if (nullptr == everyDayScheduler) {
		everyDayScheduler.reset(new EveryDayScheduler());
	}

	if (nullptr == fixedAmountScheduler) {
		fixedAmountScheduler = make_shared<FixedAmountScheduler>(
				Temperature::getInstance().getTemperatureForecast(),
				Temperature::getInstance().getTemperatureHistory()
			);
	}

	if (nullptr == fixedPeriodScheduler) {
		fixedPeriodScheduler = make_shared<FixedPeriodScheduler>(
				Temperature::getInstance().getTemperatureForecast(),
				Temperature::getInstance().getTemperatureHistory()
			);
	}

	if (nullptr == hotWeatherScheduler) {
		hotWeatherScheduler = make_shared<HotWeatherScheduler>(
				Temperature::getInstance().getTemperatureHistory()
			);
	}

	if (nullptr == runTimes) {
		runTimes.reset(new RunTimeContainer());
	}

	if (nullptr == startTimes) {
		startTimes.reset(new StartTimeContainer());
	}

	return shared_ptr<Program>(new Program(
			disabled, 
			name,
			adjustment,
			schedulerType,
			periodicScheduler,
			weeklyScheduler,
			everyDayScheduler,
			fixedAmountScheduler,
			fixedPeriodScheduler,
			hotWeatherScheduler,
			runTimes,
			startTimes));
}

nlohmann::json Program::saveTo() const {
	nlohmann::json result;
	result["fixedAmount"] = fixedAmountScheduler->saveTo();
	result["fixedPeriod"] = fixedPeriodScheduler->saveTo();
	result["hotWeather"] = hotWeatherScheduler->saveTo();
	return result;
}

void Program::loadFrom(const nlohmann::json& values) {
	auto it = values.find("fixedAmount");
	if (values.end() != it) {
		fixedAmountScheduler->loadFrom(it.value());
	}

	it = values.find("fixedPeriod");
	if (values.end() != it) {
		fixedPeriodScheduler->loadFrom(it.value());
	}

	it = values.find("hotWeather");
	if (values.end() != it) {
		hotWeatherScheduler->loadFrom(it.value());
	}
}
