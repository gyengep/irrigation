#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/StartTimeImpl.h"
#include "Logic/ProgramContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Model/IrrigationDocument.h"


IdType RestView::getStartTimeId(const KeyValue& pathParameters) {
	return IdType::from_string(pathParameters.at("startTimeId"));
}

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<HttpResponse> RestView::onGetStartTimeList(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve startTime container";

	try {
		const IdType programId = getProgramId(pathParameters);
		const std::list<StartTimeDTO> startTimeDtoList = getStartTimeDTOList(irrigationDocument, programId);

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(startTimeDtoList)).
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

std::unique_ptr<HttpResponse> RestView::onPostStartTimeList(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not create startTime container";

	try {
		const StartTimeDTO startTimeDto = dtoReader->loadStartTime(std::string(request.getUploadData()->data(), request.getUploadData()->size()));
		const IdType programId = getProgramId(pathParameters);

		const auto result = postStartTimeList(irrigationDocument, programId, startTimeDto);

		const IdType startTimeId = result.first;
		const std::string text = result.second;

		LOGGER.debug("Program[%s].StartTime[%s] is added: %s",
				programId.toString().c_str(),
				startTimeId.toString().c_str(),
				text.c_str()
			);

		return HttpResponse::Builder().
				setStatus(201, "Created").
				addHeader("Location", getStartTimeUrl(programId, startTimeId)).
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

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<HttpResponse> RestView::onGetStartTime(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve startTime";

	try {
		const IdType programId = getProgramId(pathParameters);
		const IdType startTimeId = getStartTimeId(pathParameters);
		const StartTimeDTO startTimeDto = getStartTimeDTO(irrigationDocument, programId, startTimeId);

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(startTimeDto)).
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

std::unique_ptr<HttpResponse> RestView::onPatchStartTime(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not modify startTime";

	try {
		const IdType programId = getProgramId(pathParameters);
		const IdType startTimeId = getStartTimeId(pathParameters);
		const StartTimeDTO startTimeDto = dtoReader->loadStartTime(std::string(request.getUploadData()->data(), request.getUploadData()->size()));

		const std::string text = patchStartTime(irrigationDocument, programId, startTimeId, startTimeDto);

		LOGGER.debug("Program[%s].StartTime[%s] is modified: %s",
				programId.toString().c_str(),
				startTimeId.toString().c_str(),
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

std::unique_ptr<HttpResponse> RestView::onDeleteStartTime(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not delete startTime";

	try {
		const IdType programId = getProgramId(pathParameters);
		const IdType startTimeId = getStartTimeId(pathParameters);

		deleteStartTime(irrigationDocument, programId, startTimeId);

		LOGGER.debug("Program[%s].StartTime[%s] is deleted",
				programId.toString().c_str(),
				startTimeId.toString().c_str()
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
