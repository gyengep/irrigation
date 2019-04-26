#pragma once
#include <string>
#include <memory>
#include "Logger/Logger.h"


class LogWriter {
public:
	virtual ~LogWriter() = default;
	virtual std::string contentType() const = 0;
	virtual std::string toString(const std::deque<Logger::Entry>& logEntries) const = 0;
};
