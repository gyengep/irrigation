#pragma once
#include <atomic>
#include <ctime>
#include <memory>
#include <mutex>

class IrrigationDocument;
class ZoneHandler;



class Application {

	static std::unique_ptr<Application> instance;
	static std::mutex createMutex;

	std::unique_ptr<IrrigationDocument> document;
	std::atomic_bool isTerminated;

	//void saveDocument(const std::string& fileName) const;

	void initGpio();
	void initDocument();

	std::chrono::milliseconds getDiffBetweenSystemClockAndSteadyClock();
	std::chrono::milliseconds abs(const std::chrono::milliseconds& ms);

protected:

	Application();

public:
	virtual ~Application();

	void init();
	void start();
	void stop();

	void terminate();
	
	std::time_t getTime() const;

	static Application& getInstance();
	static std::string readFile(const std::string& fileName);
	//static void writeFile(const std::string& fileName, const std::string& text);
};
