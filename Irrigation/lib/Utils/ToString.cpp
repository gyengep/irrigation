#include "ToString.h"
#include <iomanip>


std::string toCelsius(float temperature) {
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << temperature << " C";
	return oss.str();
}

std::string toPercent(unsigned value) {
	return std::to_string(value) + "%";
}

std::string toPercent(int value) {
	return std::to_string(value) + "%";
}
