#include "StartTimeImpl.h"
#include "Exceptions/Exceptions.h"
#include <iomanip>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

StartTimePtr StartTimeImplFactory::create() const {
	return std::make_shared<StartTimeImpl>();
}

///////////////////////////////////////////////////////////////////////////////

StartTimeImpl::StartTimeImpl() :
	StartTimeImpl(0, 0)
{
}

StartTimeImpl::StartTimeImpl(unsigned hour, unsigned minute) {
	set(hour, minute);
}

StartTimeImpl::~StartTimeImpl() {
}

unsigned StartTimeImpl::getHours() const {
	return hour;
}

unsigned StartTimeImpl::getMinutes() const {
	return minute;
}

void StartTimeImpl::set(unsigned hour, unsigned minute) {
	if (hour >= 24) {
		throw ValueOutOfBoundsException(
				"StartTimeImpl hour shall be less than " + std::to_string(24) +
				", while actual value is " + std::to_string(hour));
	}

	if (minute >= 60) {
		throw ValueOutOfBoundsException(
				"StartTimeImpl minute shall be less than " + std::to_string(60) +
				", while actual value is " + std::to_string(minute));
	}

	this->hour = hour;
	this->minute = minute;
}

bool StartTimeImpl::less(const StartTime& other) const {
	return ((60 * getHours() + getMinutes()) < (60 * other.getHours() + other.getMinutes()));
}

StartTimeDTO StartTimeImpl::toStartTimeDto() const {
	return StartTimeDTO(getHours(), getMinutes());
}

void StartTimeImpl::updateFromStartTimeDto(const StartTimeDTO& startTimeDTO) {
	if (startTimeDTO.hasHours() || startTimeDTO.hasMinutes()) {
		unsigned hour = 0;
		unsigned minute = 0;

		if (startTimeDTO.hasHours()) {
			hour = startTimeDTO.getHours();
		}

		if (startTimeDTO.hasMinutes()) {
			minute = startTimeDTO.getMinutes();
		}

		set(hour, minute);
	}
}

std::string StartTimeImpl::toString() const {
	std::ostringstream oss;
	oss << "StartTime{";
	oss << std::setfill('0') << std::setw(2) << getHours() << ":";
	oss << std::setfill('0') << std::setw(2) << getMinutes();
	oss << "}";
	return oss.str();
}

std::string StartTimeImpl::toShortString() const {
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << getHours() << ":";
	oss << std::setfill('0') << std::setw(2) << getMinutes();
	return oss.str();
}
