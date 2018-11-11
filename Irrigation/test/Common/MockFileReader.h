#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Utils/FileReaderWriter.h"



class MockFileReader : public FileReader {
public:
	MOCK_CONST_METHOD1(read, std::string(const std::string& fileName));
};

///////////////////////////////////////////////////////////////////////////////

class MockFileReaderWriterFactory : public FileReaderWriterFactory {
	mutable std::unique_ptr<FileReader> reader;

public:
	MockFileReaderWriterFactory(std::unique_ptr<FileReader>&& reader) : reader(move(reader)) {}
	virtual std::unique_ptr<FileReader> createFileReader() const override { return move(reader); }
	virtual std::unique_ptr<FileWriter> createFileWriter() const override { return std::unique_ptr<FileWriter>(); }
};

