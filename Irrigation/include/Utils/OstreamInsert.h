#pragma once
#include <ostream>


#define OSTREAM_INSERT(CLASS) 												\
inline std::ostream& operator<<(std::ostream& os, const CLASS& obj) {	\
	os << obj.toString();												\
	return os;															\
}
