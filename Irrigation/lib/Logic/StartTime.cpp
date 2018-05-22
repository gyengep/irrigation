#include "StartTime.h"
#include <stdexcept>

using namespace std;


StartTime::StartTime(unsigned hour, unsigned min, unsigned sec)  :
	enabled(true)
{
	setValue(sec + 60 * (min + 60 * hour));
}

StartTime::StartTime() :
	enabled(true)
{
	setValue(0);
}

StartTime::~StartTime() {
}

unsigned StartTime::getValue() const {
	return secOfDay;
}

void StartTime::setValue(unsigned secOfDay) {
	if (secOfDay > maxSeconds) {
		throw domain_error("StartTime is out of range");
	}

	this->secOfDay = secOfDay;
}

void StartTime::setValue(unsigned hour, unsigned min) {
	setValue(((hour * 60) + min) * 60);
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
