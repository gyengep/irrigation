#include "RunTime.h"
#include <stdexcept>



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
		throw std::domain_error("Runtime is out of range");
	}

	this->seconds = seconds;
}
