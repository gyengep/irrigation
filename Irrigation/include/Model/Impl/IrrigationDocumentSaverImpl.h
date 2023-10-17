#pragma once
#include <memory>
#include "DtoReaderWriter/DtoReaderWriter.h"
#include "Utils/FileWriter.h"
#include "Model/IrrigationDocument.h"



class IrrigationDocumentSaverImpl : public IrrigationDocument::Saver {
	const std::shared_ptr<DtoWriter> dtoWriter;
	const std::shared_ptr<FileWriterFactory> fileWriterFactory;

public:
	IrrigationDocumentSaverImpl(
			const std::shared_ptr<DtoWriter>& dtoWriter,
			const std::shared_ptr<FileWriterFactory>& fileWriterFactory
		);
	virtual ~IrrigationDocumentSaverImpl();

	virtual void save(const IrrigationDocument& document) override;
};
