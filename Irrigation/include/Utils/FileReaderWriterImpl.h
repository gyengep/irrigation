#pragma once
#include <string>
#include "FileReaderWriter.h"


class FileReaderImpl : public FileReader {
public:
	virtual std::string read(const std::string& fileName) const override;
};

class FileWriterImpl : public FileWriter {
public:
	virtual void write(const std::string& fileName, const std::string& text) const override;
};

class FileReaderWriterFactoryImpl : public FileReaderWriterFactory {
public:
	virtual std::unique_ptr<FileReader> createFileReader() const override {
		return std::unique_ptr<FileReader>(new FileReaderImpl());
	}

	virtual std::unique_ptr<FileWriter> createFileWriter() const override {
		return std::unique_ptr<FileWriter>(new FileWriterImpl());
	}
};
