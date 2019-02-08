#include "Program.h"
#include "RunTime.h"
#include "RunTimeContainer.h"
#include "StartTime.h"
#include "StartTimeContainer.h"
#include <sstream>
#include "Logger/Logger.h"
#include "Schedulers/PeriodicScheduler.h"
#include "Schedulers/WeeklyScheduler.h"

using namespace std;


Program::Program() :
	name(),
	schedulerType(SchedulerType::WEEKLY),
	periodicScheduler(new PeriodicScheduler()),
	weeklyScheduler(new WeeklyScheduler()),
	runTimes(new RunTimeContainer()),
	startTimes(new StartTimeContainer())
{
}

Program::Program(const Program& other) :
	name(other.getName()),
	schedulerType(other.getSchedulerType()),
	periodicScheduler(new PeriodicScheduler(other.getPeriodicScheduler())),
	weeklyScheduler(new WeeklyScheduler(other.getWeeklyScheduler())),
	runTimes(new RunTimeContainer(other.getRunTimes())),
	startTimes(new StartTimeContainer(other.getStartTimes()))
{
}

Program::Program(const string& name, SchedulerType schedulerType,
	shared_ptr<PeriodicScheduler> periodicScheduler,
	shared_ptr<WeeklyScheduler> weeklyScheduler,
	shared_ptr<RunTimeContainer> runTimes,
	shared_ptr<StartTimeContainer> startTimes)
{
	this->name = name;
	this->schedulerType = schedulerType;
	this->periodicScheduler = periodicScheduler;
	this->weeklyScheduler = weeklyScheduler;
	this->runTimes = runTimes;
	this->startTimes = startTimes;
}

Program::~Program() {
}

bool Program::operator== (const Program& other) const {
	return (getName() == other.getName() &&
			getSchedulerType() == other.getSchedulerType() &&
			getPeriodicScheduler() == other.getPeriodicScheduler() &&
			getWeeklyScheduler() == other.getWeeklyScheduler() &&
			getRunTimes() == other.getRunTimes() &&
			getStartTimes() == other.getStartTimes());
}

const string& Program::getName() const {
	return name;
}

void Program::setName(const string& name) {
	this->name = name;
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
	default:
		throw invalid_argument("Program::getCurrentScheduler(): unknown SchedulerType " + to_string(static_cast<unsigned>(schedulerType)));
	}
}

bool Program::isScheduled(const tm& timeinfo) const {

	for (auto& startTimeAndIdPair : *startTimes) {
		const StartTime& startTime = *startTimeAndIdPair.second;
		if (startTime.equals(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec)) {
			return getCurrentScheduler().isDayScheduled(timeinfo);
		}
	}

	return false;
}

ProgramDTO Program::toProgramDto() const {
	return ProgramDTO(name, to_string(getSchedulerType()),
			getPeriodicScheduler().toPeriodicSchedulerDto(),
			getWeeklyScheduler().toWeeklySchedulerDto(),
			getRunTimes().toRunTimeDtoList(),
			getStartTimes().toStartTimeDtoList());
}

void Program::updateFromProgramDto(const ProgramDTO& programDTO) {
	if (programDTO.hasName()) {
		setName(programDTO.getName());
	}

	if (programDTO.hasSchedulerType()) {
		const static string periodicSchedulerText = to_string(SchedulerType::PERIODIC);
		const static string weeklySchedulerText = to_string(SchedulerType::WEEKLY);

		if (periodicSchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::PERIODIC);
		} else if (weeklySchedulerText == programDTO.getSchedulerType()) {
			setSchedulerType(SchedulerType::WEEKLY);
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
	os << "schedulerType=\"" << to_string(program.getSchedulerType()) << "\", ";
	os << "schedulers=[" <<
			to_string(program.getPeriodicScheduler()) << ", " <<
			to_string(program.getWeeklyScheduler()) << "], ";
	os << "runTimes=" << to_string(program.getRunTimes()) << ", ";
	os << "startTimes=" << to_string(program.getStartTimes());
	os << "}";
	return os;
}

///////////////////////////////////////////////////////////////////////////////

Program::Builder::Builder() :
	schedulerType(SchedulerType::WEEKLY)
{
}

Program::Builder::~Builder() {
}

Program::Builder& Program::Builder::setName(const string& name) {
	this->name = name;
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

	if (nullptr == runTimes) {
		runTimes.reset(new RunTimeContainer());
	}

	if (nullptr == startTimes) {
		startTimes.reset(new StartTimeContainer());
	}

	return shared_ptr<Program>(new Program(name, schedulerType,
			periodicScheduler,
			weeklyScheduler,
			runTimes,
			startTimes));
}
