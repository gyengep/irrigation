#include "StartTime.h"
#include <stdexcept>



StartTime::StartTime() :
	secOfDay(0),
	enabled(false)
{
}

StartTime::~StartTime() {
}

unsigned StartTime::getValue() const {
	return secOfDay;
}

void StartTime::setValue(unsigned secOfDay) {
	if (secOfDay > maxSeconds) {
		throw std::domain_error("Starttime is out of range");
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
