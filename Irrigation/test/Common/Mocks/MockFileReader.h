#pragma once
#include <gmock/gmock.h>
#include "Utils/FileReader.h"


class MockFileReader : public FileReader {
public:
	MOCK_CONST_METHOD0(read, std::string());
};
