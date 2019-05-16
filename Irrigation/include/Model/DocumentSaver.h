#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

class DocumentDTO;
class DtoReader;
class DtoWriter;
class FileReader;
class FileWriter;
class IrrigationDocument;


class DocumentSaver {
public:
	class DtoWriterFactory;
	class FileWriterFactory;

private:
	std::condition_variable condition;
	std::mutex mtx;
	std::thread workerThread;

	std::shared_ptr<IrrigationDocument> irrigationDocument;
	std::shared_ptr<DtoWriterFactory> dtoWriterFactory;
	std::shared_ptr<FileWriterFactory> fileWriterFactory;
	bool terminated;

	void workerFunc();

public:
	DocumentSaver(
		std::shared_ptr<IrrigationDocument> irrigationDocument,
		std::shared_ptr<DtoWriterFactory> dtoWriterFactory,
		std::shared_ptr<FileWriterFactory> fileWriterFactory
	);
	virtual ~DocumentSaver();

	void saveIfModified();
	void load(std::shared_ptr<DtoReader> dtoReader, std::shared_ptr<FileReader> fileReader);

	void start();
	void stop();
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
