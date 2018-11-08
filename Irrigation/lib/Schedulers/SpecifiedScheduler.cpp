#include "SpecifiedScheduler.h"
#include <ctime>
#include <list>
#include <sstream>
#include "Exceptions/Exceptions.h"
#include "Utils/ToString.h"

using namespace std;



unique_ptr<SpecifiedScheduler> SchedulerFactory::createSpecifiedScheduler() const {
	return unique_ptr<SpecifiedScheduler>(new SpecifiedScheduler());
}

///////////////////////////////////////////////////////////////////////////////

SpecifiedScheduler::SpecifiedScheduler() :
	adjustment(100)
{
	days.fill(false);
}

SpecifiedScheduler::~SpecifiedScheduler() {
}

void SpecifiedScheduler::checkIndex(size_t day) const {
	if (days.size() <= day) {
		throw IndexOutOfBoundsException(
				"Day index shall be less than " + to_string(days.size()) +
				", while actual value is " + to_string(day));
	}
}

void SpecifiedScheduler::setAdjustment(unsigned adjustment) {
	this->adjustment = adjustment;
}

unsigned SpecifiedScheduler::getAdjustment() const {
	return adjustment;
}

void SpecifiedScheduler::enableDay(size_t day, bool enable) {
	checkIndex(day);
	days[day] = enable;
}

bool SpecifiedScheduler::isDayEnabled(size_t day) const {
	checkIndex(day);
	return days[day];
}

bool SpecifiedScheduler::isDayScheduled(const tm& timeinfo) const {
	const size_t weekDay = (size_t)timeinfo.tm_wday;

	checkIndex(weekDay);
	return days[weekDay];
}

SpecifiedSchedulerDTO SpecifiedScheduler::getSpecifiedSchedulerDTO() const {
	return SpecifiedSchedulerDTO(adjustment, list<bool>(days.begin(), days.end()));
}

void SpecifiedScheduler::updateFromDTO(const SpecifiedSchedulerDTO& schedulerDTO) {
	if (schedulerDTO.hasAdjustment()) {
		setAdjustment(schedulerDTO.getAdjustment());
	}

	if (schedulerDTO.hasValues()) {
		const size_t maxIndex = min(days.size(), schedulerDTO.getValues().size());
		for (size_t i = 0; i < maxIndex; ++i) {
			auto it = next(schedulerDTO.getValues().begin(), i);
			days[i] = *it;
		}
	}
}

string to_string(const SpecifiedScheduler& specifiedScheduler) {
	ostringstream o;
	o << "SpecifiedScheduler{";
	o << "adjustment=" << specifiedScheduler.getAdjustment() << "% ";
	o << "values=" << to_string(specifiedScheduler.days.begin(), specifiedScheduler.days.end());
	o << "}";
	return o.str();
}
