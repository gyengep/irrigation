#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "DtoReaderWriter/DtoReaderWriterFactory.h"
#include "Model/IrrigationDocument.h"
#include "Utils/FileReaderWriter.h"

class MockDtoReader : public DtoReader {
public:
	MOCK_CONST_METHOD1(loadDocument, DocumentDTO(const std::string&));
	MOCK_CONST_METHOD1(loadProgram, ProgramDTO(const std::string&));
	MOCK_CONST_METHOD1(loadRunTime, RunTimeDTO(const std::string&));
	MOCK_CONST_METHOD1(loadStartTime, StartTimeDTO(const std::string&));
	MOCK_CONST_METHOD1(loadSpecifiedScheduler, SpecifiedSchedulerDTO(const std::string&));
};

class MockDtoReaderWriterFactory : public DtoReaderWriterFactory {
	mutable std::unique_ptr<DtoReader> reader;

public:
	MockDtoReaderWriterFactory(std::unique_ptr<DtoReader>&& reader) : reader(move(reader)) {}
	virtual std::unique_ptr<DtoReader> createDtoReader() const override { return move(reader); }
	virtual std::unique_ptr<DtoWriter> createDtoWriter() const override { return std::unique_ptr<DtoWriter>(); }
};

///////////////////////////////////////////////////////////////////////////////

class MockFileReader : public FileReader {
public:
	MOCK_CONST_METHOD1(read, std::string(const std::string& fileName));
};

class MockFileReaderWriterFactory : public FileReaderWriterFactory {
	mutable std::unique_ptr<FileReader> reader;

public:
	MockFileReaderWriterFactory(std::unique_ptr<FileReader>&& reader) : reader(move(reader)) {}
	virtual std::unique_ptr<FileReader> createFileReader() const override { return move(reader); }
	virtual std::unique_ptr<FileWriter> createFileWriter() const override { return std::unique_ptr<FileWriter>(); }
};

///////////////////////////////////////////////////////////////////////////////

class IrrigationDocumentTest : public ::testing::Test {
protected:

	std::unique_ptr<IrrigationDocument> document;

    virtual void SetUp();
    virtual void TearDown();

public:
    void waitAndUnlock(IrrigationDocument* document, unsigned waitMs);
};
