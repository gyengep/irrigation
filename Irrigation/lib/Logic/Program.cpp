#include "Program.h"

///////////////////////////////////////////////////////////////////////////////

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

