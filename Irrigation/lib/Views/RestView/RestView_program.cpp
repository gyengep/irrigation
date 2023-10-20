#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"
#include "Model/IrrigationDocument.h"


IdType RestView::getProgramId(const KeyValue& pathParameters) {
	return IdType::from_string(pathParameters.at("programId"));
}

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<HttpResponse> RestView::onGetProgramList(const HttpRequest& request, const KeyValue& pathParameters) {

	const std::list<ProgramDto> programDtoList = getProgramDtoList(irrigationDocument);

	return HttpResponse::Builder().
			setStatus(200, "OK").
			setBody(dtoWriter->save(programDtoList, "/programlist.xsl")).
			addHeader("Content-Type", "application/xml").
			build();
}

std::unique_ptr<HttpResponse> RestView::onPostProgramList(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not create program";

	try {
		const ProgramDto programDto = dtoReader->loadProgram(std::string(request.getUploadData()->data(), request.getUploadData()->size()));
		const std::pair<IdType, std::string> result = postProgramList(irrigationDocument, programDto);

		const IdType& programId = result.first;
		const std::string& text = result.second;

		LOGGER.debug("Program[%s] is added: %s",
				programId.toString().c_str(),
				text.c_str()
			);

		return HttpResponse::Builder().
				setStatus(201, "Created").
				addHeader("Location", getProgramUrl(programId)).
				build();

	} catch (const ParserException& e) {
		LOGGER.warning(logMessage, e);
		throw RestBadRequest(restService->getErrorWriter(), e.what());
	}
}

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<HttpResponse> RestView::onGetProgram(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve program";

	try {
		const IdType programId = getProgramId(pathParameters);
		const ProgramDto programDto = getProgramDto(irrigationDocument, programId);

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(programDto, "/program.xsl")).
				addHeader("Content-Type", "application/xml").
				build();

	} catch (const NoSuchElementException& e) {
		LOGGER.warning(logMessage, e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	} catch (const IllegalArgumentException& e) {
		LOGGER.warning(logMessage, e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}

std::unique_ptr<HttpResponse> RestView::onPatchProgram(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not modify program";

	try {
		const IdType programId = getProgramId(pathParameters);
		const ProgramDto programDto = dtoReader->loadProgram(std::string(request.getUploadData()->data(), request.getUploadData()->size()));
		const std::string text = patchProgram(irrigationDocument, programId, programDto);

		LOGGER.debug("Program[%s] is modified: %s",
				programId.toString().c_str(),
				text.c_str()
			);

		return HttpResponse::Builder().
				setStatus(204, "No Content").
				build();

	} catch (const NoSuchElementException& e) {
		LOGGER.warning(logMessage, e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	} catch (const IllegalArgumentException& e) {
		LOGGER.warning(logMessage, e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	} catch (const ParserException& e) {
		LOGGER.warning(logMessage, e);
		throw RestBadRequest(restService->getErrorWriter(), e.what());
	}
}

std::unique_ptr<HttpResponse> RestView::onDeleteProgram(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not delete program";

	try {
		const IdType programId = getProgramId(pathParameters);
		deleteProgram(irrigationDocument, programId);

		LOGGER.debug("Program[%s] is deleted",
				programId.toString().c_str()
			);

		return HttpResponse::Builder().
				setStatus(204, "No Content").
				build();

	} catch (const NoSuchElementException& e) {
		LOGGER.warning(logMessage, e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	} catch (const IllegalArgumentException& e) {
		LOGGER.warning(logMessage, e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}

