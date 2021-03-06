#pragma once
#include "Application.h"
#include "DTO/DocumentDTO.h"
#include "Temperature/Temperature.h"

class IrrigationDocument;
class DocumentSaver;


class IrrigationApplication : public Application {
	std::shared_ptr<IrrigationDocument> irrigationDocument;
	//std::shared_ptr<Temperature> temperature;
	std::unique_ptr<DocumentSaver> documentSaver;

	void initEmail();
	void initGpio();
	void initTemperature();
	void initDocument();
	void uninitEmail();
	void uninitTemperature();
	void uninitDocument();
	void setMyDefaults();

	virtual void onInitialize() override;
	virtual void onTerminate() override;

protected:
	IrrigationApplication();

public:
	virtual ~IrrigationApplication();
	
	static IrrigationApplication& getInstance();
	static std::string getVersion();
};
