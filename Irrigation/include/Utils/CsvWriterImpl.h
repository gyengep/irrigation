#pragma once
#include <ostream>
#include <memory>
#include <string>
#include <vector>
#include "CsvWriter.h"


class CsvWriterImpl : public CsvWriter {
	static const char separator = ',';

	std::shared_ptr<std::ostream> output;

public:
	CsvWriterImpl(const std::shared_ptr<std::ostream>& output);
	virtual ~CsvWriterImpl();

	virtual void append(const std::vector<std::string>& values) override;
};
