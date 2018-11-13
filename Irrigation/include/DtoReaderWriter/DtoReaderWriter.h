#pragma once
#include <memory>
#include <string>

class DocumentDTO;
class ProgramDTO;
class RunTimeDTO;
class StartTimeDTO;
class WeeklySchedulerDTO;


class DtoReader {
public:
	virtual ~DtoReader() = default;
	virtual DocumentDTO loadDocument(const std::string& text) const = 0;
	virtual ProgramDTO loadProgram(const std::string& text) const = 0;
	virtual RunTimeDTO loadRunTime(const std::string& text) const = 0;
	virtual StartTimeDTO loadStartTime(const std::string& text) const = 0;
	virtual WeeklySchedulerDTO loadWeeklyScheduler(const std::string& text) const = 0;
};


class DtoWriter {
public:
	virtual ~DtoWriter() = default;
	virtual std::string save(const DocumentDTO& document) = 0;
	virtual std::string save(const ProgramDTO& program) = 0;
	virtual std::string save(const RunTimeDTO& runTime) = 0;
	virtual std::string save(const StartTimeDTO& startTime) = 0;
	virtual std::string save(const WeeklySchedulerDTO& scheduler) = 0;
};


class DtoReaderWriterFactory {
public:
	virtual ~DtoReaderWriterFactory() = default;
	virtual std::unique_ptr<DtoReader> createDtoReader() const = 0;
	virtual std::unique_ptr<DtoWriter> createDtoWriter() const = 0;
};