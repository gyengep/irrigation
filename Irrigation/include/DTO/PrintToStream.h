#pragma once
#include <list>
#include <ostream>



#define PRINT_STR(OS, NAME, TEXT)				\
	OS << NAME << "=";							\
	if (TEXT) {									\
		OS << "\"" << TEXT->c_str() << "\"";	\
	} else {									\
		OS << "<nullptr>";						\
	}

#define PRINT_PTR(OS, NAME, PTR)				\
	OS << NAME << "=";							\
	if (PTR) {									\
		OS << *PTR;								\
	} else {									\
		OS << "<nullptr>";						\
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
