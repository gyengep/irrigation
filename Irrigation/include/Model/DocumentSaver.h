#pragma once
#include <memory>
#include "Utils/Runnable.h"
#include "Utils/Thread.h"

class DocumentDTO;
class DtoReader;
class DtoWriter;
class FileReader;
class FileWriter;
class IrrigationDocument;


class DocumentSaver : public Runnable {
public:
	class DtoWriterFactory;
	class FileWriterFactory;

private:
	std::unique_ptr<Thread> timerThread;

	std::shared_ptr<IrrigationDocument> irrigationDocument;
	std::shared_ptr<DtoWriterFactory> dtoWriterFactory;
	std::shared_ptr<FileWriterFactory> fileWriterFactory;

public:
	DocumentSaver(
		std::shared_ptr<IrrigationDocument> irrigationDocument,
		std::shared_ptr<DtoWriterFactory> dtoWriterFactory,
		std::shared_ptr<FileWriterFactory> fileWriterFactory
	);
	virtual ~DocumentSaver();

	void saveIfModified();
	void load(std::shared_ptr<DtoReader> dtoReader, std::shared_ptr<FileReader> fileReader);

	void startTimer();
	void stopTimer();

	virtual void run() override;
};

class DocumentSaver::DtoWriterFactory {
public:
	virtual ~DtoWriterFactory() = default;
	virtual std::shared_ptr<DtoWriter> create() = 0;
};

class DocumentSaver::FileWriterFactory {
public:
	virtual ~FileWriterFactory() = default;
	virtual std::shared_ptr<FileWriter> create() = 0;
};
