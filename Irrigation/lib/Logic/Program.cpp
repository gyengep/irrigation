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
	periodicScheduler(schedulerFactory.createPeriodicScheduler()),
	weeklyScheduler(schedulerFactory.createWeeklyScheduler()),
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

Program::~Program() {
}

bool Program::operator== (const Program& other) {
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

const PeriodicScheduler& Program::getPeriodicScheduler() const {
	return *periodicScheduler.get();
}

PeriodicScheduler& Program::getPeriodicScheduler() {
	return *periodicScheduler.get();
}

const WeeklyScheduler& Program::getWeeklyScheduler() const {
	return *weeklyScheduler.get();
}

WeeklyScheduler& Program::getWeeklyScheduler() {
	return *weeklyScheduler.get();
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

std::ostream& operator<<(std::ostream& os, const Program& program) {
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
