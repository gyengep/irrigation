#pragma once
#include <algorithm>
#include <map>
#include <sstream>
#include <string>

typedef std::map<std::string, std::string> KeyValue;

inline std::string createUrl(uint16_t port, const std::string& path, const KeyValue& parameters = KeyValue());

std::string createUrl(uint16_t port, const std::string& path, const KeyValue& parameters) {
	if ('/' != path[0]) {
		throw std::logic_error("The path has to start with '/'");
	}

	std::ostringstream o;

	o << "http://localhost:" << port;
	o << path;

	if (!parameters.empty()) {
		o << "?";

		for (auto it = parameters.begin(); it != parameters.end(); ++it) {
			if (parameters.begin() != it) {
				o << "&";
			}

			o << it->first << "=" << it->second;
		}
	}

	return o.str();
}
