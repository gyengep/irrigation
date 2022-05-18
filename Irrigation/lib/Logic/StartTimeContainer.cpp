#include "StartTimeContainer.h"

///////////////////////////////////////////////////////////////////////////////

bool StartTimeContainer::operator== (const StartTimeContainer& other) const {
	if (size() != other.size()) {
		return false;
	}

	auto comp = [](const value_type& a, const value_type& b) {
		return (a.first == b.first) && (a.second->operator ==(*b.second));
	};

	return equal(begin(), end(), other.begin(), comp);
}

