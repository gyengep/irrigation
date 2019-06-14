#pragma once
#include <gmock/gmock.h>
#include "Utils/CsvWriter.h"


class MockCsvWriter : public CsvWriter {
public:
	MOCK_METHOD1(append, void(const std::vector<std::string>& values));
	MOCK_CONST_METHOD0(stream, const std::shared_ptr<std::ostream>&());
};
