#pragma once
#include <memory>
#include <string>
#include "TemperatureSensor.h"

class FileReader;


class TemperatureSensor_DS18B20 : public TemperatureSensor {
	static const std::string basePath;
	static const std::string fileName;

	const std::shared_ptr<FileReader> fileReader;

public:
	TemperatureSensor_DS18B20(const std::shared_ptr<FileReader>& fileReader);
	virtual ~TemperatureSensor_DS18B20();

	virtual float readValueFromSensor() override;

	static std::string getTempSensorFileName();
	static std::shared_ptr<TemperatureSensor_DS18B20> create();
};
