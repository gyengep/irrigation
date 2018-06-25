#include "RunTime.h"
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include "Exceptions/Exceptions.h"

using namespace std;


RunTime::RunTime() {
	setValue(0);
}

RunTime::~RunTime() {
}

unsigned RunTime::getValue() const {
	return seconds;
}

void RunTime::setValue(unsigned seconds) {
	if (seconds > maxSeconds) {
		throw ValueOutOfBoundsException(
				"RunTime value shall not be greater than " + to_string(maxSeconds) +
				", while actual value is " + to_string(seconds));
	}

	this->seconds = seconds;
}

RunTimeDTO RunTime::getRunTimeDTO() const {
	return RunTimeDTO(getValue());
}

void RunTime::updateFromDTO(const RunTimeDTO& runTimeDTO) {
	if (runTimeDTO.hasValue()) {
		setValue(runTimeDTO.getValue());
	}
}

string to_string(const RunTime& runTime) {
	ostringstream o;
	o << setfill('0') << setw(2) << (runTime.getValue()/60) << ":";
	o << setfill('0') << setw(2) << (runTime.getValue()%60);
	return o.str();
}
