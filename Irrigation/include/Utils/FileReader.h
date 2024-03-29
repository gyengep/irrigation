#pragma once
#include <string>


class FileReader {
public:
	virtual ~FileReader() = default;
	virtual std::string read() const = 0;
};
