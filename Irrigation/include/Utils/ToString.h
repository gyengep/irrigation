#pragma once
#include <sstream>
#include <string>


template <class InputIterator>
std::string to_string(const InputIterator& first, const InputIterator& last);

template <typename TYPE>
std::string to_string(const TYPE& value);

template <>
inline std::string to_string(const bool& value) {
	std::ostringstream o;
	o << (value ? "true" : "false");
	return o.str();
}

template <class InputIterator>
std::string to_string(const InputIterator&  first, const InputIterator&  last) {
	std::ostringstream o;
	o << "[";
	for (InputIterator it = first; it != last; ++it) {
		if (it != first) {
			o << ", ";
		}
		o << to_string(*it);
	}
	o << "]";
	return o.str();
}
