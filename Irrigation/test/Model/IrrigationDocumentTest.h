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
	MockDtoReader* mockReader;

public:
	MockDtoReaderWriterFactory(MockDtoReader* mockReader) : mockReader(mockReader) {}
	virtual DtoReader* createDtoReader() const override { return mockReader; }
	virtual DtoWriter* createDtoWriter() const override { return NULL; }
};

///////////////////////////////////////////////////////////////////////////////

class MockFileReader : public FileReader {
public:
	MOCK_CONST_METHOD1(read, std::string(const std::string& fileName));
};

class MockFileReaderWriterFactory : public FileReaderWriterFactory {
	MockFileReader* mockReader;

public:
	MockFileReaderWriterFactory(MockFileReader* mockReader) : mockReader(mockReader) {}
	virtual FileReader* createFileReader() const override { return mockReader; }
	virtual FileWriter* createFileWriter() const override { return NULL; }
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
