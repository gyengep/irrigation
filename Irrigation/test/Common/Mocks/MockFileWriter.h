#pragma once
#include <gmock/gmock.h>
#include "Utils/FileWriter.h"


class MockFileWriter : public FileWriter {
public:
	MOCK_METHOD1(write, void(const std::string& text));
};

///////////////////////////////////////////////////////////////////////////////

class MockFileWriterFactory : public FileWriterFactory {
public:
	MOCK_METHOD1(create, std::shared_ptr<FileWriter>(FileWriter::Type));
};
