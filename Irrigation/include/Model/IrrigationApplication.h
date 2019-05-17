#pragma once
#include "Application.h"
#include "DTO/DocumentDTO.h"

class IrrigationDocument;
class DocumentSaver;
class TemperatureStatistics;


class IrrigationApplication : public Application {
	std::shared_ptr<IrrigationDocument> irrigationDocument;
	std::unique_ptr<DocumentSaver> documentSaver;

	std::unique_ptr<TemperatureStatistics> temperatureStatistics;

	void initGpio();
	void initTemperatureSensor();
	void initDocument();

	virtual void onInitialize() override;
	virtual void onTerminate() override;

protected:
	IrrigationApplication();

public:
	virtual ~IrrigationApplication();
	
	static IrrigationApplication& getInstance();
	static std::string getVersion();
};
