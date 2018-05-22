#include "SpecifiedScheduler.h"
#include "Exceptions.h"
#include <ctime>

using namespace std;


SpecifiedScheduler::SpecifiedScheduler() {
	days.fill(false);
}

SpecifiedScheduler::~SpecifiedScheduler() {
}

void SpecifiedScheduler::enableDay(Days day, bool enable) {
	if (days.size() <= day) {
		throw InvalidDayException();
	}

	days[day] = enable;
}

bool SpecifiedScheduler::isDayEnabled(SpecifiedScheduler::Days day) const {
	if (days.size() <= day) {
		throw InvalidDayException();
	}

	return days[day];
}

bool SpecifiedScheduler::isDayScheduled(const tm& timeinfo) const {
	const size_t weekDay = (size_t)timeinfo.tm_wday;

	if (days.size() <= weekDay) {
		throw InvalidDayException();
	}

	return days[weekDay];
}
