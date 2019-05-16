#pragma once
#include <string>


class TemperatureFile {
	const std::string fileName;

public:
	TemperatureFile(const std::string& filePath);
	virtual ~TemperatureFile();
};
