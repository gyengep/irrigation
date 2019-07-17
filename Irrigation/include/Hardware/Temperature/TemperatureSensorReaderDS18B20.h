#pragma once
#include <memory>
#include <string>
#include "Utils/FileReaderWriterImpl.h"
#include "TemperatureSensorReader.h"

namespace DS18B20 {

class TemperatureSensorReader : public ::TemperatureSensorReader {
	static const std::string basePath;
	static const std::string fileName;

	const std::shared_ptr<FileReader> fileReader;

	static std::string getTempSensorFileName();

public:
	TemperatureSensorReader();
	TemperatureSensorReader(const std::shared_ptr<FileReader>& fileReader);
	virtual ~TemperatureSensorReader();

	virtual float read() override;

	static float parseText(const std::string& text);
};

}
