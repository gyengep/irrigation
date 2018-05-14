#include "StartTime.h"
#include <stdexcept>



StartTime::StartTime(unsigned hour, unsigned min, unsigned sec)  :
	enabled(false)
{
	setValue(sec + 60 * (min + 60 * hour));
}

StartTime::StartTime(unsigned seconds) :
	enabled(false)
{
	setValue(seconds);
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

bool StartTime::operator== (const StartTime& other) const {
	return (getValue() == other.getValue());

}
