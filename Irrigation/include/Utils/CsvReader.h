#pragma once
#include <memory>
#include <string>
#include <vector>


class CsvReader {
public:
	virtual ~CsvReader() = default;
	virtual std::unique_ptr<std::vector<std::string>> read() = 0;
};


class CsvReaderFactory {
public:
	virtual ~CsvReaderFactory() = default;
	virtual std::shared_ptr<CsvReader> create() = 0;
};
