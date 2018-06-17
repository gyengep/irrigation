#pragma once
#include <atomic>
#include <ctime>
#include <memory>
#include <mutex>
#include "Logger/Logger.h"


class IrrigationDocument;



class Application {

	static std::unique_ptr<Application> instance;
	static std::mutex createMutex;

	static const std::string configFileName;
	static const std::string logFileName;
	static const Logger::Level logLevel;

	std::unique_ptr<IrrigationDocument> document;
	std::atomic_bool isTerminated;

	void loadDocument(const std::string& fileName);
	void saveDocument(const std::string& fileName) const;

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
	static void writeFile(const std::string& fileName, const std::string& text);
};
