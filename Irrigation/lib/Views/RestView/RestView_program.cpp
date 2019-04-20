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
	unique_lock<IrrigationDocument> lock(irrigationDocument);
	const list<ProgramDTO> programDtoList = irrigationDocument.getPrograms().toProgramDtoList();
	lock.unlock();

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

		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const IdType programId = irrigationDocument.getPrograms().insert(IdType(), program).first;

		LOGGER.debug("Program[%s] is added: %s", to_string(programId).c_str(), to_string(*program).c_str());
		lock.unlock();


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
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const IdType programId = getProgramId(pathParameters);
		const shared_ptr<Program> program = irrigationDocument.getPrograms().at(programId);
		const ProgramDTO programDto = program->toProgramDto();
		lock.unlock();

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
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const IdType programId = getProgramId(pathParameters);
		const shared_ptr<Program> program = irrigationDocument.getPrograms().at(programId);
		const ProgramDTO programDto = dtoReader->loadProgram(string(request.getUploadData()->data(), request.getUploadData()->size()));
		program->updateFromProgramDto(programDto);

		LOGGER.debug("Program[%s] is modified: %s", to_string(programId).c_str(), to_string(*program).c_str());
		lock.unlock();


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
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const IdType programId = getProgramId(pathParameters);
		irrigationDocument.getPrograms().erase(programId);
		lock.unlock();

		LOGGER.debug("Program[%s] is deleted", to_string(programId).c_str());

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

