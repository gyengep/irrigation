#pragma once

#include <limits.h>


class PathHandler {
	char buffer[PATH_MAX + 1];

public:
	PathHandler();
	~PathHandler();

	std::string createAbsolutePath(const std::string& path);
};
