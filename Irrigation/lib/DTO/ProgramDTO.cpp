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

	if (other.hasEveryDayScheduler()) {
		setEveryDayScheduler(EveryDaySchedulerDTO(other.getEveryDayScheduler()));
	}

	if (other.hasHotWeatherScheduler()) {
		setHotWeatherScheduler(HotWeatherSchedulerDTO(other.getHotWeatherScheduler()));
	}

	if (other.hasPeriodicScheduler()) {
		setPeriodicScheduler(PeriodicSchedulerDTO(other.getPeriodicScheduler()));
	}

	if (other.hasTemperatureDependentScheduler()) {
		setTemperatureDependentScheduler(TemperatureDependentSchedulerDTO(other.getTemperatureDependentScheduler()));
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

ProgramDTO::ProgramDTO(bool disabled, const string& name,
		unsigned adjustment,
		const string& schedulerType,
		EveryDaySchedulerDTO&& everyDayScheduler,
		HotWeatherSchedulerDTO&& hotWeatherScheduler,
		PeriodicSchedulerDTO&& periodicScheduler,
		TemperatureDependentSchedulerDTO&& temperatureDependentScheduler,
		WeeklySchedulerDTO&& weeklyScheduler,
		list<RunTimeDTO>&& runTimes,
		list<StartTimeDTO>&& startTimes) {

	setDisabled(disabled);
	setName(name);
	setAdjustment(adjustment);
	setSchedulerType(schedulerType);
	setEveryDayScheduler(move(everyDayScheduler));
	setHotWeatherScheduler(move(hotWeatherScheduler));
	setPeriodicScheduler(move(periodicScheduler));
	setTemperatureDependentScheduler(move(temperatureDependentScheduler));
	setWeeklyScheduler(move(weeklyScheduler));
	setRunTimes(move(runTimes));
	setStartTimes(move(startTimes));
}

bool ProgramDTO::operator== (const ProgramDTO& other) const {
	return (equalsPtr(Id.get(), other.Id.get()) &&
			equalsPtr(Disabled.get(), other.Disabled.get()) &&
			equalsPtr(Name.get(), other.Name.get()) &&
			equalsPtr(Adjustment.get(), other.Adjustment.get()) &&
			equalsPtr(SchedulerType.get(), other.SchedulerType.get()) &&
			equalsPtr(EveryDayScheduler.get(), other.EveryDayScheduler.get()) &&
			equalsPtr(HotWeatherScheduler.get(), other.HotWeatherScheduler.get()) &&
			equalsPtr(PeriodicScheduler.get(), other.PeriodicScheduler.get()) &&
			equalsPtr(TemperatureDependentScheduler.get(), other.TemperatureDependentScheduler.get()) &&
			equalsPtr(WeeklyScheduler.get(), other.WeeklyScheduler.get()) &&
			equalsPtr(RunTimes.get(), other.RunTimes.get()) &&
			equalsPtr(StartTimes.get(), other.StartTimes.get()));
}


IMPLEMENT_DTO_VALUE_COPY(ProgramDTO, unsigned, Id);
IMPLEMENT_DTO_VALUE_COPY(ProgramDTO, bool, Disabled);
IMPLEMENT_DTO_VALUE_COPY(ProgramDTO, std::string, Name);
IMPLEMENT_DTO_VALUE_COPY(ProgramDTO, unsigned, Adjustment);
IMPLEMENT_DTO_VALUE_COPY(ProgramDTO, std::string, SchedulerType);
IMPLEMENT_DTO_VALUE_MOVE(ProgramDTO, EveryDaySchedulerDTO, EveryDayScheduler);
IMPLEMENT_DTO_VALUE_MOVE(ProgramDTO, HotWeatherSchedulerDTO, HotWeatherScheduler);
IMPLEMENT_DTO_VALUE_MOVE(ProgramDTO, PeriodicSchedulerDTO, PeriodicScheduler);
IMPLEMENT_DTO_VALUE_MOVE(ProgramDTO, TemperatureDependentSchedulerDTO, TemperatureDependentScheduler);
IMPLEMENT_DTO_VALUE_MOVE(ProgramDTO, WeeklySchedulerDTO, WeeklyScheduler);
IMPLEMENT_DTO_VALUE_MOVE(ProgramDTO, std::list<RunTimeDTO>, RunTimes);
IMPLEMENT_DTO_VALUE_MOVE(ProgramDTO, std::list<StartTimeDTO>, StartTimes);


ostream& operator<<(ostream& os, const ProgramDTO& program) {
	os << "ProgramDTO{";
	PRINT_PTR(os, "id", program.Id.get());
	os << ", ";
	PRINT_PTR(os, "disabled", program.Disabled.get());
	os << ", ";
	PRINT_STR(os, "name", program.Name.get());
	os << ", ";
	PRINT_PTR(os, "adjustment", program.Adjustment.get());
	os << ", ";
	PRINT_STR(os, "schedulerType", program.SchedulerType.get());
	os << ", ";
	PRINT_PTR(os, "everyDayScheduler", program.EveryDayScheduler.get());
	os << ", ";
	PRINT_PTR(os, "hotWeatherScheduler", program.HotWeatherScheduler.get());
	os << ", ";
	PRINT_PTR(os, "periodicScheduler", program.PeriodicScheduler.get());
	os << ", ";
	PRINT_PTR(os, "temperatureDependentScheduler", program.TemperatureDependentScheduler.get());
	os << ", ";
	PRINT_PTR(os, "weeklyScheduler", program.WeeklyScheduler.get());
	os << ", ";
	PRINT_PTR(os, "runTimes", program.RunTimes.get());
	os << ", ";
	PRINT_PTR(os, "startTimes", program.StartTimes.get());
	os << "}";

	return os;
}
