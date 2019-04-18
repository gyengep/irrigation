#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Model/IrrigationDocument.h"

using namespace std;


WeeklyScheduler& RestView::getWeeklyScheduler(const std::string& programIdText) {
	return getProgram(programIdText)->getWeeklyScheduler();
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onGetWeeklyScheduler(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve weekly scheduler";

	try {
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const WeeklySchedulerDTO weeklySchedulerDto = getWeeklyScheduler(pathParameters.at("programId")).toWeeklySchedulerDto();
		lock.unlock();

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(weeklySchedulerDto)).
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

unique_ptr<HttpResponse> RestView::onPatchWeeklyScheduler(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not modify weekly scheduler";

	try {
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		WeeklyScheduler& weeklyScheduler = getWeeklyScheduler(pathParameters.at("programId"));
		const WeeklySchedulerDTO weeklySchedulerDto = dtoReader->loadWeeklyScheduler(string(request.getUploadData()->data(), request.getUploadData()->size()));
		weeklyScheduler.updateFromWeeklySchedulerDto(weeklySchedulerDto);
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
