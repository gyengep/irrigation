#pragma once
#include <deque>
#include "LogWriter.h"


class XmlLogWriter : public LogWriter {
public:
	virtual ~XmlLogWriter() = default;
	virtual std::string contentType() const override;
	virtual std::string toString(const std::deque<Logger::Entry>& logEntries) const override;
};
