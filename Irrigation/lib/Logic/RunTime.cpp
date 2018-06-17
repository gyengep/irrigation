#include "RunTime.h"
#include <stdexcept>
#include <sstream>

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
		throw out_of_range("Runtime is out of range");
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

string RunTime::toString() const {
	ostringstream o;
	o << "RunTime{seconds=" << seconds << "}";
	return o.str();
}
