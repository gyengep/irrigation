#pragma once
#include "DtoReaderWriter/DtoReaderWriter.h"
#include "WebServer/ErrorWriter.h"


class TextErrorWriter : public ErrorWriter {
public:
	virtual ~TextErrorWriter() = default;
	virtual std::string to(const std::string& errorMessage, unsigned errorCode) override;
};

class TextErrorWriterFactory : public ErrorWriterFactory {
public:
	virtual ~TextErrorWriterFactory() = default;
	virtual std::unique_ptr<ErrorWriter> create() const override;
};

class TextDtoWriter : public DtoWriter {
public:
	virtual ~TextDtoWriter() = default;
	virtual std::string save(const DocumentDTO& document) override;
	virtual std::string save(const ProgramDTO& program) override;
	virtual std::string save(const RunTimeDTO& runTime) override;
	virtual std::string save(const StartTimeDTO& startTime) override;
	virtual std::string save(const PeriodicSchedulerDTO& scheduler) override;
	virtual std::string save(const WeeklySchedulerDTO& scheduler) override;
	virtual std::string save(const std::list<ProgramDTO>& programs) override;
	virtual std::string save(const std::list<RunTimeDTO>& runTimes) override;
	virtual std::string save(const std::list<StartTimeDTO>& startTimes) override;
};

class TextReaderWriterFactory : public DtoReaderWriterFactory {
public:
	virtual ~TextReaderWriterFactory() = default;
	virtual std::unique_ptr<DtoReader> createDtoReader() const override;
	virtual std::unique_ptr<DtoWriter> createDtoWriter() const override;
};
