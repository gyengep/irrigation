#include "ToString.h"



template <>
std::string to_string(const bool& value) {
	std::ostringstream o;
	o << (value ? "true" : "false");
	return o.str();
}
