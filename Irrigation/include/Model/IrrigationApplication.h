#pragma once
#include <atomic>
#include <ctime>
#include <memory>
#include <mutex>

enum class LogLevel;
class IrrigationDocument;
class ZoneHandler;



class Application {

	static std::unique_ptr<Application> instance;
	static std::mutex createMutex;

	static const std::string configFileName;
	static const std::string logFileName;
	static const LogLevel logLevel;

	std::unique_ptr<IrrigationDocument> document;
	std::atomic_bool isTerminated;

	void saveDocument(const std::string& fileName) const;

	void initLogger();
	void initGpio();
	void initDocument();

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
