#pragma once

#include <array>
#include <atomic>
#include <memory>
#include <mutex>
#include <ostream>
#include <stdarg.h>

#define LOGGER Logger::getInstance()

using namespace std;


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
	static unique_ptr<Logger> instance;
	static mutex initMutex;
	static const array<const char*, 6> levelTexts;
	static const unsigned bufferSize = 1000;

	mutable mutex logMutex;
	char buffer[bufferSize + 1];
	atomic<Level> level;
	ostream* output;
	unique_ptr<ostream> dynamicCreatedoutput;

	Logger();
	void log(Level level, const char * format, va_list args);

public:
	virtual ~Logger();
	static Logger& getInstance();

	void setLevel(Level level);
	void setFileName(const string& fileName);
	void setOutput(ostream& output);

	bool  isLoggable(Level level) const;
	Level getLevel() const { return level; }

	void error(const char * format, ...);
	void warning(const char * format, ...);
	void info(const char * format, ...);
	void debug(const char * format, ...);
	void trace(const char * format, ...);

	static const char* getLevelText(Level level);
};
