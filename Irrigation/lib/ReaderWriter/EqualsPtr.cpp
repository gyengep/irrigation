#include "EqualsPtr.h"



template<>
bool equalsPtr(const char* p1, const char* p2) {
	if (p1 == nullptr && p2 == nullptr) {
		return true;
	}

	if (p1 != nullptr && p2 != nullptr) {
		return (strcmp(p1, p2) == 0);
	}

	return false;
}



template<>
std::ostream& operator<<(std::ostream& os, const std::list<std::string>& list) {
	os << "[";
	for(auto it = list.begin(); it != list.end(); ++it) {
		if (it != list.begin()) {
			os << ", ";
		}
    	os << "\"" << *it << "\"";
	}
	os << "]";

	return os;
}

template<>
std::ostream& operator<<(std::ostream& os, const std::list<std::string>& list);
