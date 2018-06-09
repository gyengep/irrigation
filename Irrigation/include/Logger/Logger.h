#pragma once

#include <array>
#include <atomic>
#include <memory>
#include <mutex>
#include <ostream>
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
	static std::unique_ptr<Logger> instance;
	static std::mutex initMutex;
	static const std::array<const char*, 6> levelTexts;
	static const unsigned bufferSize = 1000;

	mutable std::mutex logMutex;
	char buffer[bufferSize + 1];
	std::atomic<Level> level;
	std::unique_ptr<std::ostream> output;

	Logger();
	void log(Level level, const char * format, va_list args);

public:
	virtual ~Logger();
	static Logger& getInstance();

	void setLevel(Level level);
	void setFileName(const std::string& fileName);
	void setOutput(std::ostream* output);

	bool  isLoggable(Level level) const;
	Level getLevel() const { return level; }

	void error(const char * format, ...);
	void warning(const char * format, ...);
	void info(const char * format, ...);
	void debug(const char * format, ...);
	void trace(const char * format, ...);

	static const char* getLevelText(Level level);
};