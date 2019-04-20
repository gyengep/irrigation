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
		unique_lock<IrrigationDocument> lock(irrigationDocument);
		const IdType programId = getProgramId(pathParameters);
		const shared_ptr<Program> program = irrigationDocument.getPrograms().at(programId);
		const WeeklyScheduler& weeklyScheduler = program->getWeeklyScheduler();
		const WeeklySchedulerDTO weeklySchedulerDto = weeklyScheduler.toWeeklySchedulerDto();
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
		const IdType programId = getProgramId(pathParameters);
		const shared_ptr<Program> program = irrigationDocument.getPrograms().at(programId);
		WeeklyScheduler& weeklyScheduler = program->getWeeklyScheduler();
		const WeeklySchedulerDTO weeklySchedulerDto = dtoReader->loadWeeklyScheduler(string(request.getUploadData()->data(), request.getUploadData()->size()));
		weeklyScheduler.updateFromWeeklySchedulerDto(weeklySchedulerDto);

		LOGGER.debug("Program[%s].WeeklyScheduler is modified: %s",
				to_string(programId).c_str(),
				to_string(weeklyScheduler).c_str());

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
