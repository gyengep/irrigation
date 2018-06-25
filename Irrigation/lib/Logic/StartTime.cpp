#include "StartTime.h"
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include "Exceptions/Exceptions.h"

using namespace std;



StartTime::StartTime(unsigned hour, unsigned min, unsigned sec) {
	setValue(sec + 60 * (min + 60 * hour));
}

StartTime::StartTime() {
	setValue(0);
}

StartTime::~StartTime() {
}

unsigned StartTime::getValue() const {
	return secOfDay;
}

void StartTime::setValue(unsigned secOfDay) {
	if (secOfDay >= maxSeconds) {
		throw ValueOutOfBoundsException(
				"StartTime value shall be less than " + to_string(maxSeconds) +
				", while actual value is " + to_string(secOfDay));
	}

	this->secOfDay = secOfDay;
}

StartTime& StartTime::operator= (const StartTime& other) {
	secOfDay = other.secOfDay;
	return *this;
}

bool StartTime::operator< (const StartTime& other) const {
	return (getValue() < other.getValue());
}

bool StartTime::operator== (const StartTime& other) const {
	return (getValue() == other.getValue());
}

StartTimeDTO StartTime::getStartTimeDTO() const {
	return StartTimeDTO(getValue());
}

void StartTime::updateFromDTO(const StartTimeDTO& startTimeDTO) {
	if (startTimeDTO.hasValue()) {
		setValue(startTimeDTO.getValue());
	}
}

string to_string(const StartTime& startTime) {
	ostringstream o;
	o << setfill('0') << setw(2) << (startTime.getValue()/3600) << ":";
	o << setfill('0') << setw(2) << (startTime.getValue()%3600/60) << ":";
	o << setfill('0') << setw(2) << (startTime.getValue()%60);
	return o.str();
}
