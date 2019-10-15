#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"
#include "Schedulers/HotWeatherScheduler.h"
#include "Model/IrrigationDocument.h"

using namespace std;


unique_ptr<HttpResponse> RestView::onGetHotWeatherScheduler(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve hotWeather scheduler";

	try {
		const IdType programId = getProgramId(pathParameters);
		HotWeatherSchedulerDTO hotWeatherSchedulerDto;

		{
			unique_lock<IrrigationDocument> lock(irrigationDocument);
			hotWeatherSchedulerDto = irrigationDocument.getPrograms().at(programId)->getHotWeatherScheduler().toHotWeatherSchedulerDto();
		}

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(hotWeatherSchedulerDto)).
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

unique_ptr<HttpResponse> RestView::onPatchHotWeatherScheduler(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not modify hotWeather scheduler";

	try {
		const IdType programId = getProgramId(pathParameters);
		const HotWeatherSchedulerDTO hotWeatherSchedulerDto = dtoReader->loadHotWeatherScheduler(string(request.getUploadData()->data(), request.getUploadData()->size()));

		{
			unique_lock<IrrigationDocument> lock(irrigationDocument);
			HotWeatherScheduler& hotWeatherScheduler = irrigationDocument.getPrograms().at(programId)->getHotWeatherScheduler();

			irrigationDocument.setModified();
			hotWeatherScheduler.updateFromHotWeatherSchedulerDto(hotWeatherSchedulerDto);

			if (LOGGER.isLoggable(LogLevel::DEBUG)) {
				const HotWeatherScheduler hotWeatherSchedulerCopy(hotWeatherScheduler);
				lock.unlock();

				LOGGER.debug("Program[%s].HotWeatherScheduler is modified: %s",
						to_string(programId).c_str(),
						to_string(hotWeatherSchedulerCopy).c_str());
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
