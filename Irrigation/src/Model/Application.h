#pragma once

#include <atomic>
#include <ctime>

#include <model/IrrigationDocument.h>


class Application {
	IrrigationDocument* document;
	std::atomic_bool isTerminated;

public:
	Application();
	~Application();

	void init();
	void run();
	void uninit();
	void terminate();
	
	std::time_t getTime() const;
};

Application* getApplication();
