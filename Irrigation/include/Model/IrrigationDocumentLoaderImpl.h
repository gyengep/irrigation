#pragma once
#include <memory>
#include "DtoReaderWriter/DtoReaderWriter.h"
#include "Utils/FileReader.h"
#include "IrrigationDocument.h"



class IrrigationDocumentLoaderImpl : public IrrigationDocument::Loader {
	const std::shared_ptr<DtoReader> dtoReader;
	const std::shared_ptr<FileReader> fileReader;

public:
	IrrigationDocumentLoaderImpl(
			const std::shared_ptr<DtoReader>& dtoReader,
			const std::shared_ptr<FileReader>& fileReader
		);
	virtual ~IrrigationDocumentLoaderImpl();

	virtual bool load(IrrigationDocument& document) override;
};
