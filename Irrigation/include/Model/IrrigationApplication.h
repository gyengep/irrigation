#pragma once
#include <memory>
#include <mutex>
#include "Application.h"

class IrrigationDocument;


class IrrigationApplication : public Application {

	static std::unique_ptr<IrrigationApplication> instance;
	static std::mutex createMutex;

	std::unique_ptr<IrrigationDocument> document;

	//void saveDocument(const std::string& fileName) const;

	void initGpio();
	void initDocument();

	virtual void onInitialize() override;
	virtual void onTerminate() override;

protected:
	IrrigationApplication() = default;

public:
	virtual ~IrrigationApplication() = default;
	
	static IrrigationApplication& getInstance();
	static std::string getVersion();
};
