#pragma once
#include <string>
#include "FileReaderWriter.h"



class FileReaderImpl : public FileReader {
public:
	virtual ~FileReaderImpl() = default;
	virtual std::string read(const std::string& fileName) const override;
};


class FileWriterImpl : public FileWriter {
public:
	virtual ~FileWriterImpl() = default;
	virtual void write(const std::string& fileName, const std::string& text) const override;
};


class FileReaderWriterFactoryImpl : public FileReaderWriterFactory {
public:
	virtual ~FileReaderWriterFactoryImpl() = default;

	virtual FileReader* createFileReader() const override { return new FileReaderImpl(); }
	virtual FileWriter* createFileWriter() const override { return new FileWriterImpl(); }
};
