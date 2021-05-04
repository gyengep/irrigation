#pragma once
#include <istream>
#include "CsvReader.h"


class CsvReaderImpl : public CsvReader {
	static const char separator = ',';

	std::shared_ptr<std::istream> input;

public:
	CsvReaderImpl(const std::shared_ptr<std::istream>& input);
	virtual ~CsvReaderImpl();

	virtual std::unique_ptr<std::vector<std::string>> read() override;
};


class CsvReaderFactoryImpl : public CsvReaderFactory {
	const std::string fileName;

public:
	CsvReaderFactoryImpl(const std::string& fileName);

	virtual ~CsvReaderFactoryImpl();
	virtual std::shared_ptr<CsvReader> create() override;
};
