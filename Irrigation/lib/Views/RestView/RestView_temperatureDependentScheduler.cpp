#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"
#include "Schedulers/TemperatureDependentScheduler.h"
#include "Model/IrrigationDocument.h"

using namespace std;


unique_ptr<HttpResponse> RestView::onGetTemperatureDependentScheduler(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve temperatureDependent scheduler";

	try {
		const IdType programId = getProgramId(pathParameters);
		TemperatureDependentSchedulerDTO temperatureDependentSchedulerDto;

		{
			unique_lock<IrrigationDocument> lock(irrigationDocument);
			temperatureDependentSchedulerDto = irrigationDocument.getPrograms().at(programId)->getSchedulerContainer().getTemperatureDependentScheduler().toTemperatureDependentSchedulerDto();
		}

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(temperatureDependentSchedulerDto)).
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

unique_ptr<HttpResponse> RestView::onPatchTemperatureDependentScheduler(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not modify temperatureDependent scheduler";

	try {
		const IdType programId = getProgramId(pathParameters);
		const TemperatureDependentSchedulerDTO temperatureDependentSchedulerDto = dtoReader->loadTemperatureDependentScheduler(string(request.getUploadData()->data(), request.getUploadData()->size()));

		{
			unique_lock<IrrigationDocument> lock(irrigationDocument);
			TemperatureDependentScheduler& temperatureDependentScheduler = irrigationDocument.getPrograms().at(programId)->getSchedulerContainer().getTemperatureDependentScheduler();

			irrigationDocument.setModified();
			temperatureDependentScheduler.updateFromTemperatureDependentSchedulerDto(temperatureDependentSchedulerDto);

			if (LOGGER.isLoggable(LogLevel::DEBUG)) {
				const std::string logText = temperatureDependentScheduler.toString();
				lock.unlock();

				LOGGER.debug("Program[%s].TemperatureDependentScheduler is modified: %s",
						programId.toString().c_str(),
						logText.c_str());
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
