#include "SpecifiedScheduler.h"
#include "Exceptions.h"
#include <ctime>
#include <list>
#include <sstream>

using namespace std;



SpecifiedScheduler* SchedulerFactory::createSpecifiedScheduler() const {
	return new SpecifiedScheduler();
}

///////////////////////////////////////////////////////////////////////////////

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

SpecifiedSchedulerDTO SpecifiedScheduler::getSpecifiedSchedulerDTO() const {
	return SpecifiedSchedulerDTO(new list<bool>(days.begin(), days.end()));
}

void SpecifiedScheduler::updateFromDTO(const SpecifiedSchedulerDTO& schedulerDTO) {
	if (schedulerDTO.hasValues()) {
		const size_t maxIndex = min(days.size(), schedulerDTO.getValues().size());
		for (size_t i = 0; i < maxIndex; ++i) {
			auto it = next(schedulerDTO.getValues().begin(), i);
			days[i] = *it;
		}
	}
}

std::string SpecifiedScheduler::toString() const {
	ostringstream o;
	o << "SpecifiedScheduler{values=[";
	for (auto it = days.begin(); it != days.end(); ++it) {
		if (it != days.begin()) {
			o << ", ";
		}
		o << (*it ? "true" : "false");
	}
	o << "]}";
	return o.str();
}
