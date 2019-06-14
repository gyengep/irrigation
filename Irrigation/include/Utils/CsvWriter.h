#pragma once
#include <memory>
#include <ostream>
#include <string>
#include <vector>


class CsvWriter {
public:
	virtual ~CsvWriter() = default;
	virtual void append(const std::vector<std::string>& values) = 0;
	virtual const std::shared_ptr<std::ostream>& stream() const = 0;
};


class CsvWriterFactory {
public:
	virtual ~CsvWriterFactory() = default;
	virtual std::shared_ptr<CsvWriter> create(const std::shared_ptr<std::ostream>& output) = 0;
};
