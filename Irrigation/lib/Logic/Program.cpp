#include "Program.h"
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "StartTime.h"
#include "StartTimeContainer.h"
#include <sstream>
#include "Logger/Logger.h"
#include "Schedulers/PeriodicScheduler.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Schedulers/EveryDayScheduler.h"
#include "Utils/ToString.h"

using namespace std;


Program::Program() :
	Program(false, "", 100, SchedulerType::WEEKLY,
		shared_ptr<PeriodicScheduler>(new PeriodicScheduler()),
		shared_ptr<WeeklyScheduler>(new WeeklyScheduler()),
		shared_ptr<EveryDayScheduler>(new EveryDayScheduler()),
		shared_ptr<RunTimeContainer>(new RunTimeContainer()),
		shared_ptr<StartTimeContainer>(new StartTimeContainer())
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
	shared_ptr<PeriodicScheduler> periodicScheduler, shared_ptr<WeeklyScheduler> weeklyScheduler, shared_ptr<EveryDayScheduler> everyDayScheduler,
	shared_ptr<RunTimeContainer> runTimes, shared_ptr<StartTimeContainer> startTimes) :
	disabled(disabled),
	name(name),
	adjustment(adjustment),
	schedulerType(schedulerType),
	periodicScheduler(periodicScheduler),
	weeklyScheduler(weeklyScheduler),
	everyDayScheduler(everyDayScheduler),
	runTimes(runTimes),
	startTimes(startTimes)
{
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
	this->schedulerType = schedulerType;
}

SchedulerType Program::getSchedulerType(void) const {
	return schedulerType;
}

const Scheduler& Program::getCurrentScheduler() const {
	switch (schedulerType) {
	case SchedulerType::WEEKLY:
		return getWeeklyScheduler();
	case SchedulerType::PERIODIC:
		return getPeriodicScheduler();
	case SchedulerType::EVERY_DAY:
		return getEveryDayScheduler();
	default:
		throw invalid_argument("Program::getCurrentScheduler(): unknown SchedulerType " + to_string(static_cast<unsigned>(schedulerType)));
	}
}

bool Program::isScheduled(const tm& timeinfo) const {
	if (false == disabled) {
		for (auto& startTimeAndIdPair : *startTimes) {
			const StartTime& startTime = *startTimeAndIdPair.second;
			if (startTime.equals(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec)) {
				return getCurrentScheduler().isDayScheduled(timeinfo);
			}
		}
	}

	return false;
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

		if (periodicSchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::PERIODIC);
		} else if (weeklySchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::WEEKLY);
		} else if (everyDaySchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::EVERY_DAY);
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
			to_string(program.getEveryDayScheduler()) << "], ";
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
			runTimes,
			startTimes));
}
