#pragma once
#include "DTO/DocumentDTO.h"
#include "Temperature/TemperatureHandler.h"
#include "Application.h"
#include "ShutdownManagerImpl.h"
#include "IrrigationDocument.h"

class DocumentSaver;


class IrrigationApplication : public Application {
	std::shared_ptr<IrrigationDocument> irrigationDocument;
	//std::shared_ptr<TemperatureHandler> temperature;
	std::unique_ptr<DocumentSaver> documentSaver;
	std::shared_ptr<ShutdownManagerImpl> shutdownManager;

	void initEmail();
	void initGpio();
	void initTemperature();
	void initDocument();
	void initShutdownManager();
	void uninitEmail();
	void uninitTemperature();
	void uninitDocument();
	void uninitShutdownManager();
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
