#pragma once
#include <string>
#include <map>
#include <vector>

typedef std::vector<std::string> Path;
typedef std::map<std::string, std::string> Parameters;


class PathTemplate {
	const std::string originalPath;
	Path path;
	std::vector<bool> isPathSegmentVariable;

	void checkVariables();

	static bool isVariable(const std::string& str);
	static void removeVariableDelimiter(std::string& str);

public:
	PathTemplate(const std::string& path);
	~PathTemplate();

	static void split(const std::string& pathStr, Path& result);

	bool evaluate(const Path& otherPath, Parameters& parameters) const;
};
