#pragma once
#include <memory>
#include <string>
#include "Utils/FileReaderWriterImpl.h"


class DS18B20TemperatureSensor {
	static const std::string basePath;
	static const std::string fileName;

	const std::shared_ptr<FileReader> fileReader;

	static std::string getTempSensorFileName();

public:
	DS18B20TemperatureSensor();
	DS18B20TemperatureSensor(const std::shared_ptr<FileReader>& fileReader);
	virtual ~DS18B20TemperatureSensor();

	float read();

	static float parseText(const std::string& text);
};
