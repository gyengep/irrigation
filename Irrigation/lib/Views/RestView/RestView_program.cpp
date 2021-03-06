#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"
#include "Model/IrrigationDocument.h"

using namespace std;


IdType RestView::getProgramId(const KeyValue& pathParameters) {
	return IdType::from_string(pathParameters.at("programId"));
}

bool RestView::includeContainers(const KeyValue& keyValue) {
	bool result = false;
	auto it = keyValue.find("include-containers");
	if (keyValue.end() != it) {
		result = (it->second == "true");
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onGetProgramList(const HttpRequest& request, const KeyValue& pathParameters) {

	list<ProgramDTO> programDtoList;

	{
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		programDtoList = irrigationDocument.getPrograms().toProgramDtoList();
	}

	return HttpResponse::Builder().
			setStatus(200, "OK").
			setBody(dtoWriter->save(programDtoList, includeContainers(request.getParameters()))).
			addHeader("Content-Type", "application/xml").
			build();
}

unique_ptr<HttpResponse> RestView::onPostProgramList(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not create program";

	try {
		const ProgramDTO programDto = dtoReader->loadProgram(string(request.getUploadData()->data(), request.getUploadData()->size()));
		const shared_ptr<Program> program(new Program(programDto));
		const IdType programId;

		{
			unique_lock<IrrigationDocument> lock(irrigationDocument);
			irrigationDocument.setModified();
			irrigationDocument.getPrograms().insert(programId, program);

			if (LOGGER.isLoggable(LogLevel::DEBUG)) {
				const Program programCopy(*program);
				lock.unlock();

				LOGGER.debug("Program[%s] is added: %s",
						to_string(programId).c_str(),
						to_string(programCopy).c_str());
			}
		}


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

unique_ptr<HttpResponse> RestView::onGetProgram(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve program";

	try {
		const IdType programId = getProgramId(pathParameters);
		ProgramDTO programDto;

		{
			unique_lock<IrrigationDocument> lock(irrigationDocument);
			programDto = irrigationDocument.getPrograms().at(programId)->toProgramDto();
		}

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(programDto, includeContainers(request.getParameters()))).
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

unique_ptr<HttpResponse> RestView::onPatchProgram(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not modify program";

	try {
		const IdType programId = getProgramId(pathParameters);
		const ProgramDTO programDto = dtoReader->loadProgram(string(request.getUploadData()->data(), request.getUploadData()->size()));

		{
			unique_lock<IrrigationDocument> lock(irrigationDocument);
			const shared_ptr<Program> program = irrigationDocument.getPrograms().at(programId);

			irrigationDocument.setModified();
			program->updateFromProgramDto(programDto);

			if (LOGGER.isLoggable(LogLevel::DEBUG)) {
				const Program programCopy(*program);
				lock.unlock();

				LOGGER.debug("Program[%s] is modified: %s",
						to_string(programId).c_str(),
						to_string(programCopy).c_str());
			}
		}


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

unique_ptr<HttpResponse> RestView::onDeleteProgram(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not delete program";

	try {
		const IdType programId = getProgramId(pathParameters);

		{
			unique_lock<IrrigationDocument> lock(irrigationDocument);
			irrigationDocument.setModified();
			irrigationDocument.getPrograms().erase(programId);
		}

		LOGGER.debug("Program[%s] is deleted",
				to_string(programId).c_str());

		return HttpResponse::Builder().
				setStatus(200, "OK").
				build();

	} catch (const NoSuchElementException& e) {
		LOGGER.warning(logMessage, e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	} catch (const IllegalArgumentException& e) {
		LOGGER.warning(logMessage, e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}

