#pragma once
#include <memory>
#include <string>
#include <vector>


class CsvWriter {
public:
	virtual ~CsvWriter() = default;
	virtual void append(const std::vector<std::string>& values) = 0;
};


class CsvWriterFactory {
public:
	virtual ~CsvWriterFactory() = default;
	virtual std::shared_ptr<CsvWriter> create() = 0;
};
