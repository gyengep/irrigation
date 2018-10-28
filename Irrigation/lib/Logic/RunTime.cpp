#include "RunTime.h"
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include "Exceptions/Exceptions.h"

using namespace std;


RunTime::RunTime() :
	RunTime(0)
{
}

RunTime::RunTime(unsigned seconds) :
	seconds(seconds)
{
}

RunTime::RunTime(const RunTime& other) {
	this->operator =(other);
}

RunTime::~RunTime() {
}

unsigned RunTime::getSeconds() const {
	return seconds;
}

void RunTime::setSeconds(unsigned seconds) {
	if (seconds > maxSeconds) {
		throw ValueOutOfBoundsException(
				"RunTime value shall not be greater than " + to_string(maxSeconds) +
				", while actual value is " + to_string(seconds));
	}

	this->seconds = seconds;
}

RunTime& RunTime::operator= (const RunTime& other) {
	if (this != &other) {
		seconds = other.seconds;
	}

	return *this;
}

bool RunTime::operator== (const RunTime& other) const {
	return (seconds == other.seconds);
}

RunTimeDTO RunTime::getRunTimeDTO() const {
	return RunTimeDTO(seconds / 60, seconds % 60);
}

void RunTime::updateFromDTO(const RunTimeDTO& runTimeDTO) {
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
	ostringstream o;
	o << setfill('0') << setw(2) << (runTime.getSeconds() / 60) << ":";
	o << setfill('0') << setw(2) << (runTime.getSeconds() % 60);
	return o.str();
}
