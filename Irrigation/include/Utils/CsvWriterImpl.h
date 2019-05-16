#pragma once
#include <fstream>
#include <memory>
#include "CsvWriter.h"



class CsvWriterImpl: public CsvWriter {
	std::shared_ptr<std::ofstream> ofs;

public:
	CsvWriterImpl(const std::shared_ptr<std::ofstream>& ofs, char separator);
	virtual ~CsvWriterImpl();

	virtual void append(const std::vector<std::string>& values) override;
};
