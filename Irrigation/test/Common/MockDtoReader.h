#pragma once
#include <gmock/gmock.h>
#include <memory>
#include <DtoReaderWriter/DtoReaderWriter.h>



class MockDtoReader : public DtoReader {
public:
	MOCK_CONST_METHOD1(loadDocument, DocumentDTO(const std::string&));
	MOCK_CONST_METHOD1(loadProgram, ProgramDTO(const std::string&));
	MOCK_CONST_METHOD1(loadRunTime, RunTimeDTO(const std::string&));
	MOCK_CONST_METHOD1(loadStartTime, StartTimeDTO(const std::string&));
	MOCK_CONST_METHOD1(loadPeriodicScheduler, PeriodicSchedulerDTO(const std::string&));
	MOCK_CONST_METHOD1(loadWeeklyScheduler, WeeklySchedulerDTO(const std::string&));
};

///////////////////////////////////////////////////////////////////////////////

class MockDtoReaderWriterFactory : public DtoReaderWriterFactory {
	mutable std::unique_ptr<DtoReader> reader;

public:
	MockDtoReaderWriterFactory(std::unique_ptr<DtoReader>&& reader) : reader(move(reader)) {}
	virtual std::unique_ptr<DtoReader> createDtoReader() const override { return move(reader); }
	virtual std::unique_ptr<DtoWriter> createDtoWriter() const override { return std::unique_ptr<DtoWriter>(); }
};

