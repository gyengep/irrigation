#pragma once
#include <gmock/gmock.h>
#include <list>
#include "Utils/CsvReader.h"


class MockCsvReader : public CsvReader {
public:
	MOCK_METHOD0(read, std::unique_ptr<std::vector<std::string>>());
};


class MockCsvReaderFactory : public CsvReaderFactory {
	std::list<std::shared_ptr<MockCsvReader>> mockCsvReaders;

public:
	MockCsvReaderFactory(const std::shared_ptr<MockCsvReader>& mockCsvReader) {
		mockCsvReaders.push_back(mockCsvReader);
	}

	virtual std::shared_ptr<CsvReader> create() override {
		auto result = mockCsvReaders.front();
		mockCsvReaders.pop_front();
		return result;
	}
};
