#pragma once
#include <memory>
#include <string>


class FileReader {
public:
	virtual ~FileReader() = default;
	virtual std::string read(const std::string& fileName) const = 0;
};


class FileWriter {
public:
	virtual ~FileWriter() = default;
	virtual void write(const std::string& fileName, const std::string& text) const = 0;
};


class FileReaderWriterFactory {
public:
	virtual ~FileReaderWriterFactory() = default;
	virtual std::unique_ptr<FileReader> createFileReader() const = 0;
	virtual std::unique_ptr<FileWriter> createFileWriter() const = 0;
};
