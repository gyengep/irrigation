#include "IrrigationDocumentLoaderImpl.h"
#include "DTO/DocumentDTO.h"
#include "Logger/Logger.h"
#include <string>


IrrigationDocumentLoaderImpl::IrrigationDocumentLoaderImpl(
	const std::shared_ptr<DtoReader>& dtoReader,
	const std::shared_ptr<FileReader>& fileReader
) :
	dtoReader(dtoReader),
	fileReader(fileReader)
{
}

IrrigationDocumentLoaderImpl::~IrrigationDocumentLoaderImpl() {
}

bool IrrigationDocumentLoaderImpl::load(IrrigationDocument& document) {
	const std::string documentDtoAsText = fileReader->read();
	const DocumentDTO documentDto = dtoReader->loadDocument(documentDtoAsText);

	document.updateFromDocumentDto(documentDto);
	LOGGER.debug("Configuration successfully loaded");
	return true;
}
