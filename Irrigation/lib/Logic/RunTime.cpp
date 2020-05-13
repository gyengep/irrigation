#include "RunTime.h"
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include "Exceptions/Exceptions.h"

using namespace std;


RunTime::RunTime() : RunTime(0) {
}

RunTime::RunTime(unsigned seconds) :
	milliSeconds(seconds * 1000)
{
}

RunTime::RunTime(const chrono::seconds& seconds) : RunTime(seconds.count()) {
}

bool RunTime::operator== (const RunTime& other) const {
	return (milliSeconds == other.milliSeconds);
}

unsigned RunTime::getSeconds() const {
	return milliSeconds / 1000;
}

unsigned RunTime::getMilliSeconds() const {
	return milliSeconds;
}

void RunTime::setSeconds(unsigned seconds) {
	if (seconds > maxSeconds) {
		throw ValueOutOfBoundsException(
				"RunTime value shall not be greater than " + to_string(maxSeconds) +
				", while actual value is " + to_string(seconds));
	}

	this->milliSeconds = seconds * 1000;
}

void RunTime::setMilliSeconds(unsigned milliSeconds) {
	if (milliSeconds > maxSeconds * 1000) {
		throw ValueOutOfBoundsException(
				"RunTime value shall not be greater than " + to_string(maxSeconds * 1000) + " ms" +
				", while actual value is " + to_string(milliSeconds));
	}

	this->milliSeconds = milliSeconds;
}

RunTimeDTO RunTime::toRunTimeDto() const {
	const unsigned seconds = getSeconds();
	return RunTimeDTO(seconds / 60, seconds % 60);
}

void RunTime::updateFromRunTimeDto(const RunTimeDTO& runTimeDTO) {
	if (runTimeDTO.hasMinutes() || runTimeDTO.hasSeconds()) {
		unsigned minutes = 0;
		unsigned seconds = 0;

		if (runTimeDTO.hasMinutes()) {
			minutes = runTimeDTO.getMinutes();
		}

		if (runTimeDTO.hasSeconds()) {
			seconds = runTimeDTO.getSeconds();
		}

		setSeconds(60 * minutes + seconds);
	}
}

string to_string(const RunTime& runTime) {
	ostringstream oss;
	oss << runTime;
	return oss.str();
}

ostream& operator<<(ostream& os, const RunTime& runTime) {
	os << setfill('0') << setw(2) << (runTime.getSeconds() / 60) << ":";
	os << setfill('0') << setw(2) << (runTime.getSeconds() % 60);
	return os;
}
