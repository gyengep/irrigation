#include "RunTime.h"
#include <stdexcept>

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
		throw domain_error("Runtime is out of range");
	}

	this->seconds = seconds;
}
