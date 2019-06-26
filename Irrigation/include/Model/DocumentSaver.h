#pragma once
#include <memory>
#include "Utils/Timer.h"

class DocumentDTO;
class DtoReader;
class DtoWriter;
class FileReader;
class FileWriter;
class IrrigationDocument;


class DocumentSaver : public TimerCallback {
public:
	class DtoWriterFactory;
	class FileWriterFactory;

private:
	Timer timer;

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
	virtual void onTimer() override;
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
