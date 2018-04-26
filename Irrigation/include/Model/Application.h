#pragma once

#include <atomic>
#include <ctime>
#include <memory>
#include "Utils/Factory.h"

class IrrigationDocument;
class ApplicationFactory;

class Application {
	friend ApplicationFactory;

	static std::unique_ptr<Application> instance;
	static std::unique_ptr<Factory<Application>> factory;
	static std::mutex createMutex;

	IrrigationDocument* document;
	std::atomic_bool isTerminated;

protected:

	Application();

public:
	virtual ~Application();

	void init();
	void run();
	void uninit();
	virtual void terminate();
	
	std::time_t getTime() const;

	static Application& getInstance();
	static void setFactory(Factory<Application>* applicationFactory);
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

class ApplicationFactory : public Factory<Application> {
public:
	virtual Application* create() { return new Application(); }
};

