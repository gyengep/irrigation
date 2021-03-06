#include "PathTemplate.h"
#include <iostream>
#include <sstream>

using namespace std;


PathTemplate::PathTemplate(const string& path) :
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
				throw invalid_argument("Invalid path: \"" + originalPath + "\"");
			}
		}
	}
}

bool PathTemplate::isVariable(const string& str) {
	return (!str.empty() && str.front() == '{' && str.back() == '}');
}

void PathTemplate::removeVariableDelimiter(string& str) {
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
			parameters.insert(make_pair(path[i], otherPath[i]));
		} else {
			if (path[i] != otherPath[i]) {
				parameters.clear();
				return false;
			}
		}
	}

	return true;
}

void PathTemplate::split(const string& pathStr, Path& result) {

	result.clear();

	if (pathStr.empty() || pathStr.front() != '/') {
		throw invalid_argument("Invalid path: \"" + pathStr + "\"");
	}

	size_t posBegin = 1, posEnd;
	while ((posEnd = pathStr.find('/', posBegin)) != string::npos) {
		if (posEnd == posBegin) {
			result.clear();
			throw invalid_argument("Invalid path: \"" + pathStr + "\"");
		}

		result.push_back(pathStr.substr(posBegin, posEnd - posBegin));
		posBegin = posEnd + 1;
	}

	result.push_back(pathStr.substr(posBegin));
}
