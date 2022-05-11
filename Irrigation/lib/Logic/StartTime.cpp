#include "StartTime.h"
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include "Exceptions/Exceptions.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

StartTimePtr StartTimeFactory::create() const {
	return std::make_shared<StartTime>();
}

///////////////////////////////////////////////////////////////////////////////

StartTime::StartTime() : StartTime(0, 0) {
}

StartTime::StartTime(unsigned hour, unsigned minute) {
	set(hour, minute);
}

StartTime::StartTime(const StartTimeDTO& startTimeDTO) : StartTime() {
	updateFromStartTimeDto(startTimeDTO);
}

StartTime::~StartTime() {
}

bool StartTime::operator< (const StartTime& other) const {
	return ((60 * hour + minute) < (60 * other.hour + other.minute));
}

bool StartTime::operator== (const StartTime& other) const {
	return (hour == other.hour &&
			minute == other.minute);
}

bool StartTime::equals(unsigned hour, unsigned minute, unsigned second) const {
	return (this->hour == hour &&
			this->minute == minute &&
			StartTime::second == second);
}

unsigned StartTime::getHours() const {
	return hour;
}

unsigned StartTime::getMinutes() const {
	return minute;
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

StartTimeDTO StartTime::toStartTimeDto() const {
	return StartTimeDTO(getHours(), getMinutes());
}

void StartTime::updateFromStartTimeDto(const StartTimeDTO& startTimeDTO) {
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

string to_string_short(const StartTime& startTime) {
	ostringstream oss;
	oss << setfill('0') << setw(2) << startTime.getHours() << ":";
	oss << setfill('0') << setw(2) << startTime.getMinutes();
	return oss.str();
}

string to_string(const StartTime& startTime) {
	ostringstream oss;
	oss << startTime;
	return oss.str();
}

ostream& operator<<(ostream& os, const StartTime& startTime) {
	os << "StartTime{";
	os << setfill('0') << setw(2) << startTime.getHours() << ":";
	os << setfill('0') << setw(2) << startTime.getMinutes();
	os << "}";
	return os;
}
