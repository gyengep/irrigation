#include "StartTime.h"
#include "Parser.h"
#include <stdexcept>

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
	if (secOfDay > maxSeconds) {
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

string StartTime::toString() const {
	return to_string(secOfDay);
}

void StartTime::fromString(const string& text) {
	unsigned value = 0;
	try {
		value = Parser::parseUnsigned(text);
	} catch(out_of_range& e) {
		throw out_of_range("StartTime is out of range");
	} catch (invalid_argument& e) {
		throw invalid_argument("StartTime string contains invalid characters");
	}

	setValue(value);
}
