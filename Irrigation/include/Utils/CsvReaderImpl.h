#pragma once
#include "CsvReader.h"


class CsvReaderImpl : public CsvReader {
	static const char separator = ',';

	std::shared_ptr<std::istream> input;

public:
	CsvReaderImpl(const std::shared_ptr<std::istream>& input);
	virtual ~CsvReaderImpl();
	virtual std::unique_ptr<std::vector<std::string>> read() override;
};


class CsvReaderImplFactory : public CsvReaderFactory {
public:
	virtual ~CsvReaderImplFactory();
	virtual std::shared_ptr<CsvReader> create(const std::shared_ptr<std::istream>& input) override;
};
