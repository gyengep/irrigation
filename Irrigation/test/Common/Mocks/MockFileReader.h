#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Utils/FileReaderWriter.h"



class MockFileReader : public FileReader {
public:
	MOCK_CONST_METHOD0(read, std::string());
};

///////////////////////////////////////////////////////////////////////////////

class MockFileWriter : public FileWriter {
public:
	MOCK_CONST_METHOD1(write, void(const std::string& text));
};
