#include "StartTime.h"
#include <stdexcept>
#include <sstream>

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
		throw out_of_range("StartTime is out of range");
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

string StartTime::toString() const {
	ostringstream o;
	o << "StartTime{secondOfDay=" << secOfDay << "}";
	return o.str();
}
