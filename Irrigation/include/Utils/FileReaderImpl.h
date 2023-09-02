#pragma once
#include <string>
#include "FileReader.h"


class FileReaderImpl : public FileReader {
	const std::string fileName;

public:
	FileReaderImpl(const std::string& fileName);
	virtual std::string read() const override;
};
