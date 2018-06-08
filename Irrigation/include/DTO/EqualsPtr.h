#pragma once
#include <string.h>
#include <list>
#include <ostream>


#define PRINT_STR(OS, NAME, TEXT)			\
	OS << NAME << "=";						\
	if (TEXT) {								\
		OS << "\"" << TEXT->c_str() << "\"";	\
	} else {								\
		OS << "<nullptr>";					\
	}

#define PRINT_PTR(OS, NAME, PTR)			\
	OS << NAME << "=";						\
	if (PTR) {								\
		OS << *PTR;							\
	} else {								\
		OS << "<nullptr>";					\
	}


template<typename TYPE>
bool equalsPtr(const TYPE* p1, const TYPE* p2) {
	if (p1 == nullptr && p2 == nullptr) {
		return true;
	}

	if (p1 != nullptr && p2 != nullptr) {
		return (*p1 == *p2);
	}

	return false;
}


template<typename TYPE>
std::ostream& operator<<(std::ostream& os, const std::list<TYPE>& list) {
	os << "[";
	for(auto it = list.begin(); it != list.end(); ++it) {
		if (it != list.begin()) {
			os << ", ";
		}
    	os << *it;
	}
	os << "]";

	return os;
}

