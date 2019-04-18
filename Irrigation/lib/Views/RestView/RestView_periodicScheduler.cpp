#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Schedulers/PeriodicScheduler.h"
#include "Model/IrrigationDocument.h"

using namespace std;


PeriodicScheduler& RestView::getPeriodicScheduler(const std::string& programIdText) {
	return getProgram(programIdText)->getPeriodicScheduler();
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onGetPeriodicScheduler(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve periodic scheduler";

	try {
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const PeriodicSchedulerDTO periodicSchedulerDto = getPeriodicScheduler(pathParameters.at("programId")).toPeriodicSchedulerDto();
		lock.unlock();

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(periodicSchedulerDto)).
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

unique_ptr<HttpResponse> RestView::onPatchPeriodicScheduler(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not modify periodic scheduler";

	try {
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		PeriodicScheduler& periodicScheduler = getPeriodicScheduler(pathParameters.at("programId"));
		const PeriodicSchedulerDTO periodicSchedulerDto = dtoReader->loadPeriodicScheduler(string(request.getUploadData()->data(), request.getUploadData()->size()));
		periodicScheduler.updateFromPeriodicSchedulerDto(periodicSchedulerDto);
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
