#pragma once
#include <atomic>
#include <ctime>
#include <memory>
#include <mutex>


class IrrigationDocument;



class Application {

	static std::unique_ptr<Application> instance;
	static std::mutex createMutex;

	std::unique_ptr<IrrigationDocument> document;
	std::atomic_bool isTerminated;

	void init();
	void cleanup();

protected:

	Application();

public:
	virtual ~Application();

	void run();
	void terminate();
	
	std::time_t getTime() const;

	static Application& getInstance();
};
