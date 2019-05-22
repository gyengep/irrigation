#pragma once
#include <string>
#include <vector>


class CsvWriter {
public:
	virtual ~CsvWriter() = default;
	virtual void append(const std::vector<std::string>& values) = 0;
};
