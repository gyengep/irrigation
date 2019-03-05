#include "PathTemplate.h"
#include <iostream>
#include <sstream>

using namespace std;


PathTemplate::PathTemplate(const std::string& path) :
	originalPath(path)
{
	split(originalPath, this->path);
	checkVariables();
}

PathTemplate::~PathTemplate() {
}

void PathTemplate::checkVariables() {
	isPathSegmentVariable.resize(path.size());

	for (size_t i = 0; i < path.size(); ++i) {
		if (true == (isPathSegmentVariable[i] = isVariable(path[i]))) {
			removeVariableDelimiter(path[i]);
			if (path[i].empty()) {
				throw std::invalid_argument("Invalid path: \"" + originalPath + "\"");
			}
		}
	}
}

bool PathTemplate::isVariable(const std::string& str) {
	return (!str.empty() && str.front() == '{' && str.back() == '}');
}

void PathTemplate::removeVariableDelimiter(std::string& str) {
	str.erase(str.begin());
	str.erase(str.size() - 1);
}

bool PathTemplate::evaluate(const Path& otherPath, KeyValue& parameters) const {
	parameters.clear();

	if (path.size() != otherPath.size()) {
		return false;
	}

	for (size_t i = 0; i < path.size(); ++i) {
		if (isPathSegmentVariable[i]) {
			parameters.insert(std::make_pair(path[i], otherPath[i]));
		} else {
			if (path[i] != otherPath[i]) {
				parameters.clear();
				return false;
			}
		}
	}

	return true;
}

void PathTemplate::split(const std::string& pathStr, Path& result) {

	result.clear();

	if (pathStr.empty() || pathStr.front() != '/' || (pathStr.size() > 1 && pathStr.back() == '/')) {
		throw std::invalid_argument("Invalid path: \"" + pathStr + "\"");
	}

	std::string path(pathStr, 1);

	std::istringstream f(path);
	std::string token;
	while (std::getline(f, token, '/')) {

		if (token.empty()) {
			result.clear();
			throw std::invalid_argument("Invalid path: \"" + pathStr + "\"");
		}

		result.push_back(token);
	}
}
