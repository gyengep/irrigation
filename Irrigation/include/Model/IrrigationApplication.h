#pragma once
#include "Application.h"
#include "DTO/DocumentDTO.h"

class IrrigationDocument;
class DocumentSaver;


class IrrigationApplication : public Application {
	std::shared_ptr<IrrigationDocument> irrigationDocument;
	std::unique_ptr<DocumentSaver> documentSaver;

	void initGpio();
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
