#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Model/IrrigationDocument.h"

using namespace std;


unique_ptr<HttpResponse> RestView::onGetWeeklyScheduler(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve weekly scheduler";

	try {
		const IdType programId = getProgramId(pathParameters);
		const WeeklySchedulerDto weeklySchedulerDto = getWeeklySchedulerDto(irrigationDocument, programId);

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
		const IdType programId = getProgramId(pathParameters);
		const WeeklySchedulerDto weeklySchedulerDto = dtoReader->loadWeeklySchedulerDto(string(request.getUploadData()->data(), request.getUploadData()->size()));
		const std::string text = patchWeeklyScheduler(irrigationDocument, programId, weeklySchedulerDto);

		LOGGER.debug("Program[%s].WeeklyScheduler is modified: %s",
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
