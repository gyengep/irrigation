#pragma once



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
