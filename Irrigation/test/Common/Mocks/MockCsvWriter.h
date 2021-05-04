#pragma once
#include <gmock/gmock.h>
#include <list>
#include "Utils/CsvWriter.h"


class MockCsvWriter : public CsvWriter {
public:
	MOCK_METHOD1(append, void(const std::vector<std::string>& values));
};


class MockCsvWriterFactory : public CsvWriterFactory {
	std::list<std::shared_ptr<MockCsvWriter>> mockCsvWriters;

public:
	MockCsvWriterFactory(const std::shared_ptr<MockCsvWriter>& mockCsvWriter) {
		mockCsvWriters.push_back(mockCsvWriter);
	}

	virtual std::shared_ptr<CsvWriter> create() override {
		auto result = mockCsvWriters.front();
		mockCsvWriters.pop_front();
		return result;
	}
};
