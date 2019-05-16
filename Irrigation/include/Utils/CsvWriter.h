#pragma once
#include <string>
#include <vector>


class CsvWriter {
protected:
	const char separator;

public:
	CsvWriter(char separator) : separator(separator) {}

	virtual ~CsvWriter() = default;
	virtual void append(const std::vector<std::string>& values) = 0;
};
