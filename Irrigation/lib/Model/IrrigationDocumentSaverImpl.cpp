#include "Impl/IrrigationDocumentSaverImpl.h"
#include "DTO/DocumentDTO.h"
#include "Logger/Logger.h"
#include <string>

IrrigationDocumentSaverImpl::IrrigationDocumentSaverImpl(
	const std::shared_ptr<DtoWriter>& dtoWriter,
	const std::shared_ptr<FileWriterFactory>& fileWriterFactory
) :
	dtoWriter(dtoWriter),
	fileWriterFactory(fileWriterFactory)
{
}

IrrigationDocumentSaverImpl::~IrrigationDocumentSaverImpl() {
}

void IrrigationDocumentSaverImpl::save(const IrrigationDocument& document) {
	const DocumentDTO documentDto = document.toDocumentDto();
	const std::string documentDtoAsText = dtoWriter->save(documentDto);

	fileWriterFactory->create(FileWriter::Type::TRUNCATE)->write(documentDtoAsText);
	LOGGER.debug("Configuration successfully saved");
}

