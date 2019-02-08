#pragma once
#include <memory>
#include <mutex>
#include "Application.h"

class IrrigationDocument;


class IrrigationApplication : public Application {
	std::shared_ptr<IrrigationDocument> document;

	//void saveDocument(const std::string& fileName) const;

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
