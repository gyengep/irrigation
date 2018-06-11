#include "Scheduler.h"
#include "SpecifiedScheduler.h"



SpecifiedScheduler* SchedulerFactory::createSpecifiedScheduler() const {
	return new SpecifiedScheduler();
}
