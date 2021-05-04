#pragma once
#include <ostream>
#include "CsvWriter.h"


class CsvWriterImpl : public CsvWriter {
	static const char separator = ',';

	std::shared_ptr<std::ostream> output;

public:
	CsvWriterImpl(const std::shared_ptr<std::ostream>& output);
	virtual ~CsvWriterImpl();

	virtual void append(const std::vector<std::string>& values) override;
};


class CsvWriterFactoryImpl : public CsvWriterFactory {
	const std::string fileName;

public:
	CsvWriterFactoryImpl(const std::string& fileName);

	virtual ~CsvWriterFactoryImpl();
	virtual std::shared_ptr<CsvWriter> create() override;
};
