#pragma once
#include <memory>
#include <string>
#include "TemperatureSensorReader.h"

class FileReader;


class TemperatureSensorReader_DS18B20 : public TemperatureSensorReader {
	static const std::string basePath;
	static const std::string fileName;

	const std::shared_ptr<FileReader> fileReader;

	static std::string getTempSensorFileName();

public:
	TemperatureSensorReader_DS18B20();
	TemperatureSensorReader_DS18B20(const std::shared_ptr<FileReader>& fileReader);
	virtual ~TemperatureSensorReader_DS18B20();

	virtual float read() override;
};
