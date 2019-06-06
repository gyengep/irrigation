#pragma once
#include "CsvWriter.h"


class CsvWriterImpl : public CsvWriter {
	static const char separator = ',';

	std::shared_ptr<std::ostream> output;

public:
	CsvWriterImpl(const std::shared_ptr<std::ostream>& output);
	virtual ~CsvWriterImpl();

	virtual void append(const std::vector<std::string>& values) override;
};


class CsvWriterImplFactory : public CsvWriterFactory {
public:
	virtual ~CsvWriterImplFactory();
	virtual std::shared_ptr<CsvWriter> create(const std::shared_ptr<std::ostream>& output) override;
};
