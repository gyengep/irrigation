#include "common.h"
#include "PathHandler.h"

#include <stdlib.h>

PathHandler::PathHandler() {
	buffer[PATH_MAX] = '\0';
}

PathHandler::~PathHandler() {
}

std::string PathHandler::createAbsolutePath(const std::string& path) {
	char* result = realpath(path.c_str(), buffer);
	if (NULL == result) {
		return std::string();
	}

	return std::string(result);
}
