#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/StartTime.h"
#include "Logic/ProgramContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Model/IrrigationDocument.h"

using namespace std;


IdType RestView::getStartTimeId(const KeyValue& pathParameters) {
	return IdType::from_string(pathParameters.at("startTimeId"));
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onGetStartTimeList(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve startTime container";

	try {
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const IdType programId = getProgramId(pathParameters);
		const shared_ptr<Program> program = irrigationDocument.getPrograms().at(programId);
		const list<StartTimeDTO> startTimeDtoList = program->getStartTimes().toStartTimeDtoList();
		lock.unlock();

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

unique_ptr<HttpResponse> RestView::onPostStartTimeList(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not create startTime container";

	try {
		const StartTimeDTO startTimeDto = dtoReader->loadStartTime(string(request.getUploadData()->data(), request.getUploadData()->size()));
		const shared_ptr<StartTime> startTime(new StartTime(startTimeDto));
		const IdType programId = getProgramId(pathParameters);

		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const shared_ptr<Program> program = irrigationDocument.getPrograms().at(programId);
		const IdType startTimeId = program->getStartTimes().insert(IdType(), startTime).first;

		LOGGER.debug("Program[%s].StartTime[%s] is added: %s",
				to_string(programId).c_str(),
				to_string(startTimeId).c_str(),
				to_string(*startTime).c_str());

		lock.unlock();

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

unique_ptr<HttpResponse> RestView::onGetStartTime(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve startTime";

	try {
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const IdType programId = getProgramId(pathParameters);
		const IdType startTimeId = getStartTimeId(pathParameters);
		const shared_ptr<Program> program = irrigationDocument.getPrograms().at(programId);
		const StartTimeDTO startTimeDto = program->getStartTimes().at(startTimeId)->toStartTimeDto();
		lock.unlock();

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

unique_ptr<HttpResponse> RestView::onPatchStartTime(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not modify startTime";

	try {
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const IdType programId = getProgramId(pathParameters);
		const IdType startTimeId = getStartTimeId(pathParameters);
		const shared_ptr<Program> program = irrigationDocument.getPrograms().at(programId);
		shared_ptr<StartTime> startTime = program->getStartTimes().at(startTimeId);
		const StartTimeDTO startTimeDto = dtoReader->loadStartTime(string(request.getUploadData()->data(), request.getUploadData()->size()));
		startTime->updateFromStartTimeDto(startTimeDto);

		LOGGER.debug("Program[%s].StartTime[%s] is modified: %s",
				to_string(programId).c_str(),
				to_string(startTimeId).c_str(),
				to_string(*startTime).c_str());

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

unique_ptr<HttpResponse> RestView::onDeleteStartTime(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not delete startTime";

	try {
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const IdType programId = getProgramId(pathParameters);
		const IdType startTimeId = getStartTimeId(pathParameters);
		const shared_ptr<Program> program = irrigationDocument.getPrograms().at(programId);
		program->getStartTimes().erase(startTimeId);

		LOGGER.debug("Program[%s].StartTime[%s] is deleted",
				to_string(programId).c_str(),
				to_string(startTimeId).c_str());

		lock.unlock();

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
