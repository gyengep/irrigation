#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTimeContainer.h"
#include "Model/IrrigationDocument.h"

using namespace std;


unique_ptr<HttpResponse> RestView::onGetRunTimeList(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not retrieve runTime container";

	try {
		const IdType programId = getProgramId(pathParameters);
		list<RunTimeDTO> runTimeDtoList;

		{
			unique_lock<IrrigationDocument> lock(irrigationDocument);
			runTimeDtoList = irrigationDocument.getPrograms().at(programId)->getRunTimes().toRunTimeDtoList();
		}

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(runTimeDtoList)).
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

unique_ptr<HttpResponse> RestView::onPatchRunTimeList(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not modify runTime container";

	try {
		const IdType programId = getProgramId(pathParameters);
		const list<RunTimeDTO> runTimeDtoList = dtoReader->loadRunTimeList(string(request.getUploadData()->data(), request.getUploadData()->size()));

		{
			unique_lock<IrrigationDocument> lock(irrigationDocument);
			RunTimeContainer& runTimes = irrigationDocument.getPrograms().at(programId)->getRunTimes();

			irrigationDocument.setModified();
			runTimes.updateFromRunTimeDtoList(runTimeDtoList);

			if (LOGGER.isLoggable(LogLevel::DEBUG)) {
				const RunTimeContainer runTimeContainerCopy(runTimes);
				lock.unlock();

				LOGGER.debug("Program[%s].RunTimes are modified: %s",
						to_string(programId).c_str(),
						to_string(runTimeContainerCopy).c_str());
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

