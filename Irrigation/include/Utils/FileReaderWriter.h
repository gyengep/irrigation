#pragma once
#include <memory>
#include <string>


class FileReader {
public:
	virtual ~FileReader() = default;
	virtual std::string read() const = 0;
};


class FileWriter {
public:
	virtual ~FileWriter() = default;
	virtual void write(const std::string& text) const = 0;
};
