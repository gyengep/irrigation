#include "SchedulerContainer.h"
#include "Schedulers/EveryDayScheduler.h"
#include "Schedulers/HotWeatherScheduler.h"
#include "Schedulers/TemperatureDependentScheduler.h"
#include "Schedulers/WeeklyScheduler.h"

/*
bool SchedulerContainer::operator== (const SchedulerContainer& other) const {
	return (
		getEveryDayScheduler() == other.getEveryDayScheduler() &&
		getHotWeatherScheduler() == other.getHotWeatherScheduler() &&
		getTemperatureDependentScheduler() == other.getTemperatureDependentScheduler() &&
		getWeeklyScheduler() == other.getWeeklyScheduler()
	);
}
*/
