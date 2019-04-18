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


const shared_ptr<StartTime>& RestView::getStartTime(const string& programIdText, const string& startTimeIdText) const {
	const StartTimeContainer& startTimes = getProgram(programIdText)->getStartTimes();
	return startTimes.at(stoul(startTimeIdText));
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onGetStartTimeList(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve startTime container";

	try {
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const list<StartTimeDTO> startTimeDtoList = getProgram(pathParameters.at("programId"))->getStartTimes().toStartTimeDtoList();
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
		const IdType programId = IdType::from_string(pathParameters.at("programId"));

		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const IdType startTimeId = irrigationDocument.getPrograms().at(programId)->getStartTimes().insert(IdType(), startTime).first;
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
		const StartTimeDTO startTimeDto = getStartTime(pathParameters.at("programId"), pathParameters.at("startTimeId"))->toStartTimeDto();
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
		const shared_ptr<StartTime> startTime = getStartTime(pathParameters.at("programId"), pathParameters.at("startTimeId"));
		const StartTimeDTO startTimeDto = dtoReader->loadStartTime(string(request.getUploadData()->data(), request.getUploadData()->size()));
		startTime->updateFromStartTimeDto(startTimeDto);
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
		const shared_ptr<Program> program = getProgram(pathParameters.at("programId"));
		program->getStartTimes().erase(IdType::from_string(pathParameters.at("startTimeId")));
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
