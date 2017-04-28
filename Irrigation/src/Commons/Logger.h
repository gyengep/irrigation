#pragma once

#include <atomic>
#include <fstream>
#include <mutex>
#include <stdarg.h>

#define LOGGER Logger::getInstance()

class Logger {
public:

	enum Level {
		OFF,
		ERROR,
		WARNING,
		INFO,
		DEBUG,
		TRACE
	};

private:
	static std::atomic<Logger*> instance;
	static std::mutex initMutex;
	static const unsigned bufferSize = 1000;

	char buffer[bufferSize + 1];
	std::mutex logMutex;
	std::atomic<Level> level;
	std::ofstream logFile;

	Logger();
	void log(Level level, const char * format, va_list args);

	static const char* getLevelText(Level level);

public:
	virtual ~Logger();
	static Logger& getInstance();

	void setLevel(Level level);
	void setFile(const char* fileName = NULL);

	bool isLoggable(Level level) const;

	void error(const char * format, ...);
	void warning(const char * format, ...);
	void info(const char * format, ...);
	void debug(const char * format, ...);
	void trace(const char * format, ...);
};
