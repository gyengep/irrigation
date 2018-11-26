#include "StartTime.h"
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include "Exceptions/Exceptions.h"

using namespace std;


StartTime::StartTime(unsigned hour, unsigned minute) : second(0) {
	set(hour, minute);
}

StartTime::StartTime() : StartTime(0, 0) {
}

StartTime::~StartTime() {
}

unsigned StartTime::getHours() const {
	return hour;
}

unsigned StartTime::getMinutes() const {
	return minute;
}

unsigned StartTime::getSeconds() const {
	return second;
}

void StartTime::set(unsigned hour, unsigned minute) {
	if (hour >= 24) {
		throw ValueOutOfBoundsException(
				"StartTime hour shall be less than " + to_string(24) +
				", while actual value is " + to_string(hour));
	}

	if (minute >= 60) {
		throw ValueOutOfBoundsException(
				"StartTime minute shall be less than " + to_string(60) +
				", while actual value is " + to_string(minute));
	}

	this->hour = hour;
	this->minute = minute;
}

bool StartTime::operator< (const StartTime& other) const {
	if (getHours() != other.getHours()) {
		return (getHours() < other.getHours());
	}

	if (getMinutes() != other.getMinutes()) {
		return (getMinutes() < other.getMinutes());
	}

	return (getSeconds() < other.getSeconds());
}

bool StartTime::operator== (const StartTime& other) const {
	return equals(other.getHours(), other.getMinutes(), other.getSeconds());
}

bool StartTime::equals(unsigned hour, unsigned minute, unsigned second) const {
	return (getHours() == hour &&
			getMinutes() == minute &&
			getSeconds() == second);
}

StartTimeDTO StartTime::getStartTimeDTO() const {
	if (getSeconds() != 0) {
		throw logic_error("StartTime::getStartTimeDTO(): getSeconds() != 0");
	}

	return StartTimeDTO(getHours(), getMinutes());
}

void StartTime::updateFromDTO(const StartTimeDTO& startTimeDTO) {
	if (startTimeDTO.hasHour() || startTimeDTO.hasMinute()) {
		unsigned hour = 0;
		unsigned minute = 0;

		if (startTimeDTO.hasHour()) {
			hour = startTimeDTO.getHour();
		}

		if (startTimeDTO.hasMinute()) {
			minute = startTimeDTO.getMinute();
		}

		set(hour, minute);
	}
}

string to_string(const StartTime& startTime) {
	ostringstream o;
	o << setfill('0') << setw(2) << startTime.getHours() << ":";
	o << setfill('0') << setw(2) << startTime.getMinutes() << ":";
	o << setfill('0') << setw(2) << startTime.getSeconds();
	return o.str();
}
