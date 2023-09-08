#pragma once
#include <fstream>
#include <string>
#include "FileWriter.h"


class FileWriterImpl : public FileWriter {
	std::ofstream ofs;

public:
	FileWriterImpl(const std::string& fileName, FileWriter::Type type);

	virtual ~FileWriterImpl();
	virtual void write(const std::string& text) override;
};


///////////////////////////////////////////////////////////////////////////////

class FileWriterImplFactory : public FileWriterFactory {
	const std::string fileName;

public:
	FileWriterImplFactory(const std::string& fileName);
	virtual std::shared_ptr<FileWriter> create(FileWriter::Type) override;
};
