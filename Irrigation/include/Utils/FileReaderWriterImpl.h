#pragma once
#include <string>
#include "FileReaderWriter.h"


class FileReaderImpl : public FileReader {
	const std::string fileName;

public:
	FileReaderImpl(const std::string& fileName);
	virtual std::string read() const override;
};

class FileWriterImpl : public FileWriter {
	const std::string fileName;

public:
	FileWriterImpl(const std::string& fileName);
	virtual void write(const std::string& text) const override;
};
