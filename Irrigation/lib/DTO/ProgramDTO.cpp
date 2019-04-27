#include "ProgramDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


ProgramDTO::ProgramDTO(const ProgramDTO& other) {
	if (other.hasId()) {
		setId(other.getId());
	}

	if (other.hasDisabled()) {
		setDisabled(other.getDisabled());
	}

	if (other.hasName()) {
		setName(other.getName());
	}

	if (other.hasAdjustment()) {
		setAdjustment(other.getAdjustment());
	}

	if (other.hasSchedulerType()) {
		setSchedulerType(other.getSchedulerType());
	}

	if (other.hasPeriodicScheduler()) {
		setPeriodicScheduler(PeriodicSchedulerDTO(other.getPeriodicScheduler()));
	}

	if (other.hasWeeklyScheduler()) {
		setWeeklyScheduler(WeeklySchedulerDTO(other.getWeeklyScheduler()));
	}

	if (other.hasRunTimes()) {
		setRunTimes(list<RunTimeDTO>(other.getRunTimes()));
	}

	if (other.hasStartTimes()) {
		setStartTimes(list<StartTimeDTO>(other.getStartTimes()));
	}
}

ProgramDTO::ProgramDTO(bool disabled, const std::string& name,
		unsigned adjustment,
		const string& schedulerType,
		PeriodicSchedulerDTO&& periodicScheduler,
		WeeklySchedulerDTO&& weeklyScheduler,
		list<RunTimeDTO>&& runTimes,
		list<StartTimeDTO>&& startTimes) {

	setDisabled(disabled);
	setName(name);
	setAdjustment(adjustment);
	setSchedulerType(schedulerType);
	setPeriodicScheduler(move(periodicScheduler));
	setWeeklyScheduler(move(weeklyScheduler));
	setRunTimes(move(runTimes));
	setStartTimes(move(startTimes));
}

bool ProgramDTO::operator== (const ProgramDTO& other) const {
	return (equalsPtr(id.get(), other.id.get()) &&
			equalsPtr(disabled.get(), other.disabled.get()) &&
			equalsPtr(name.get(), other.name.get()) &&
			equalsPtr(adjustment.get(), other.adjustment.get()) &&
			equalsPtr(schedulerType.get(), other.schedulerType.get()) &&
			equalsPtr(periodicScheduler.get(), other.periodicScheduler.get()) &&
			equalsPtr(weeklyScheduler.get(), other.weeklyScheduler.get()) &&
			equalsPtr(runTimes.get(), other.runTimes.get()) &&
			equalsPtr(startTimes.get(), other.startTimes.get()));
}

bool ProgramDTO::hasId() const {
	return (id.get() != nullptr);
}

bool ProgramDTO::hasDisabled() const {
	return (disabled.get() != nullptr);
}

bool ProgramDTO::hasName() const {
	return (name.get() != nullptr);
}

bool ProgramDTO::hasAdjustment() const {
	return (adjustment.get() != nullptr);
}

bool ProgramDTO::hasSchedulerType() const {
	return (schedulerType.get() != nullptr);
}

bool ProgramDTO::hasPeriodicScheduler() const {
	return (periodicScheduler.get() != nullptr);
}

bool ProgramDTO::hasWeeklyScheduler() const {
	return (weeklyScheduler.get() != nullptr);
}

bool ProgramDTO::hasRunTimes() const {
	return (runTimes.get() != nullptr);
}

bool ProgramDTO::hasStartTimes() const {
	return (startTimes.get() != nullptr);
}

unsigned ProgramDTO::getId() const {
	if (!hasId()) {
		throw logic_error("ProgramDTO::getId(): !hasId()");
	}

	return *id.get();
}

bool ProgramDTO::getDisabled() const {
	if (!hasDisabled()) {
		throw logic_error("ProgramDTO::getDisabled(): !hasDisabled()");
	}

	return *disabled.get();
}

const string& ProgramDTO::getName() const {
	if (!hasName()) {
		throw logic_error("ProgramDTO::getName(): !hasName()");
	}

	return *name.get();
}

unsigned ProgramDTO::getAdjustment() const {
	if (!hasAdjustment()) {
		throw logic_error("ProgramDTO::getAdjustment(): !hasAdjustment()");
	}

	return *adjustment.get();
}

const string& ProgramDTO::getSchedulerType() const {
	if (!hasSchedulerType()) {
		throw logic_error("ProgramDTO::getSchedulerType(): !hasSchedulerType()");
	}

	return *schedulerType.get();
}

const PeriodicSchedulerDTO& ProgramDTO::getPeriodicScheduler() const {
	if (!hasPeriodicScheduler()) {
		throw logic_error("ProgramDTO::getPeriodicScheduler(): !hasPeriodicScheduler()");
	}

	return *periodicScheduler.get();
}

const WeeklySchedulerDTO& ProgramDTO::getWeeklyScheduler() const {
	if (!hasWeeklyScheduler()) {
		throw logic_error("ProgramDTO::getWeeklyScheduler(): !hasWeeklyScheduler()");
	}

	return *weeklyScheduler.get();
}

const list<RunTimeDTO>& ProgramDTO::getRunTimes() const {
	if (!hasRunTimes()) {
		throw logic_error("ProgramDTO::getRunTimes(): !hasRunTimes()");
	}

	return *runTimes.get();
}

const list<StartTimeDTO>& ProgramDTO::getStartTimes() const {
	if (!hasStartTimes()) {
		throw logic_error("ProgramDTO::getStartTimes(): !hasStartTimes()");
	}

	return *startTimes.get();
}

ProgramDTO& ProgramDTO::setId(unsigned id) {
	this->id.reset(new unsigned(id));
	return *this;
}

ProgramDTO& ProgramDTO::setDisabled(bool disabled) {
	this->disabled.reset(new bool(disabled));
	return *this;
}

ProgramDTO& ProgramDTO::setName(const string& name) {
	this->name.reset(new string(name));
	return *this;
}

ProgramDTO& ProgramDTO::setAdjustment(unsigned adjustment) {
	this->adjustment.reset(new unsigned(adjustment));
	return *this;
}

ProgramDTO& ProgramDTO::setSchedulerType(const string& schedulerType) {
	this->schedulerType.reset(new string(schedulerType));
	return *this;
}

ProgramDTO& ProgramDTO::setPeriodicScheduler(PeriodicSchedulerDTO&& periodicScheduler) {
	this->periodicScheduler.reset(new PeriodicSchedulerDTO(move(periodicScheduler)));
	return *this;
}

ProgramDTO& ProgramDTO::setWeeklyScheduler(WeeklySchedulerDTO&& weeklyScheduler) {
	this->weeklyScheduler.reset(new WeeklySchedulerDTO(move(weeklyScheduler)));
	return *this;
}

ProgramDTO& ProgramDTO::setRunTimes(list<RunTimeDTO>&& runTimes) {
	this->runTimes.reset(new list<RunTimeDTO>(move(runTimes)));
	return *this;
}

ProgramDTO& ProgramDTO::setStartTimes(list<StartTimeDTO>&& startTimes) {
	this->startTimes.reset(new list<StartTimeDTO>(move(startTimes)));
	return *this;
}

ostream& operator<<(ostream& os, const ProgramDTO& program) {
	os << "ProgramDTO{";
	PRINT_PTR(os, "id", program.id.get());
	os << ", ";
	PRINT_PTR(os, "disabled", program.disabled.get());
	os << ", ";
	PRINT_STR(os, "name", program.name.get());
	os << ", ";
	PRINT_PTR(os, "adjustment", program.adjustment.get());
	os << ", ";
	PRINT_STR(os, "schedulerType", program.schedulerType.get());
	os << ", ";
	PRINT_PTR(os, "periodicScheduler", program.periodicScheduler.get());
	os << ", ";
	PRINT_PTR(os, "weeklyScheduler", program.weeklyScheduler.get());
	os << ", ";
	PRINT_PTR(os, "runTimes", program.runTimes.get());
	os << ", ";
	PRINT_PTR(os, "startTimes", program.startTimes.get());
	os << "}";

	return os;
}
