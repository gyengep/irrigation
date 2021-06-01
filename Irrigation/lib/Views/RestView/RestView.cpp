#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "XmlTemperatureWriter.h"
#include "XmlErrorWriter.h"
#include "XmlLogWriter.h"
#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/IdType.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/WateringController.h"
#include "Model/IrrigationDocument.h"
#include <ctime>
#include <cstring>

using namespace std;


RestView::RestView(IrrigationDocument& irrigationDocument, uint16_t port,
		const std::shared_ptr<CurrentTemperature>& currentTemperature,
		const std::shared_ptr<TemperatureForecast>& temperatureForecast,
		const std::shared_ptr<TemperatureHistory>& temperatureHistory
) :
	View(irrigationDocument),
	port(port),
	currentTemperature(currentTemperature),
	temperatureForecast(temperatureForecast),
	temperatureHistory(temperatureHistory),
	irrigationDocument(irrigationDocument)
{
	if (nullptr == currentTemperature) {
		throw std::invalid_argument("RestView::RestView() nullptr == currentTemperature");
	}

	if (nullptr == temperatureForecast) {
		throw std::invalid_argument("RestView::RestView() nullptr == temperatureForecast");
	}

	if (nullptr == temperatureHistory) {
		throw std::invalid_argument("RestView::RestView() nullptr == temperatureHistory");
	}

	restService.reset(new RestService());
	webServer.reset(new WebServer(restService, port));
	dtoReader.reset(new XmlReader());
	dtoWriter.reset(new XmlWriter());
	logWriter.reset(new XmlLogWriter());
	temperatureWriter.reset(new XmlTemperatureWriter());

	using namespace placeholders;

	restService->addPath(MHD_HTTP_METHOD_GET,  	 "/programs", bind(&RestView::onGetProgramList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_POST,   "/programs", bind(&RestView::onPostProgramList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/programs/{programId}", bind(&RestView::onGetProgram, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/programs/{programId}", bind(&RestView::onPatchProgram, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_DELETE, "/programs/{programId}", bind(&RestView::onDeleteProgram, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/programs/{programId}/runtimes", bind(&RestView::onGetRunTimeList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/programs/{programId}/runtimes", bind(&RestView::onPatchRunTimeList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/programs/{programId}/starttimes", bind(&RestView::onGetStartTimeList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_POST, 	 "/programs/{programId}/starttimes", bind(&RestView::onPostStartTimeList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,  	 "/programs/{programId}/starttimes/{startTimeId}", bind(&RestView::onGetStartTime, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/programs/{programId}/starttimes/{startTimeId}", bind(&RestView::onPatchStartTime, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_DELETE, "/programs/{programId}/starttimes/{startTimeId}", bind(&RestView::onDeleteStartTime, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/programs/{programId}/schedulers/hot-weather", bind(&RestView::onGetHotWeatherScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/programs/{programId}/schedulers/hot-weather", bind(&RestView::onPatchHotWeatherScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/programs/{programId}/schedulers/periodic", bind(&RestView::onGetPeriodicScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/programs/{programId}/schedulers/periodic", bind(&RestView::onPatchPeriodicScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/programs/{programId}/schedulers/temperature-dependent", bind(&RestView::onGetTemperatureDependentScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/programs/{programId}/schedulers/temperature-dependent", bind(&RestView::onPatchTemperatureDependentScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/programs/{programId}/schedulers/weekly", bind(&RestView::onGetWeeklyScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/programs/{programId}/schedulers/weekly", bind(&RestView::onPatchWeeklyScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/irrigation", bind(&RestView::onPatchIrrigation, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/logs", bind(&RestView::onGetLogs, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/temperature", bind(&RestView::onGetTemperature, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/temperature/forecast", bind(&RestView::onGetTemperatureForecast, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/temperature/historical", bind(&RestView::onGetTemperatureHistory, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/shutdown/poweroff", bind(&RestView::onPatchPoweroff, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/shutdown/reboot", bind(&RestView::onPatchReboot, this, _1, _2));
}

RestView::~RestView() {
}

void RestView::initialize() {
	LOGGER.debug("RestView initializing...");

	webServer->start();

	LOGGER.debug("RestView initialized");
}

void RestView::terminate() {
	LOGGER.debug("RestView terminating...");

	webServer->stop();

	LOGGER.debug("RestView terminated");
}

///////////////////////////////////////////////////////////////////////////////

string RestView::getProgramUrl(const IdType& programId) {
	return "/programs/" + to_string(programId);
}

string RestView::getStartTimeUrl(const IdType& programId, const IdType& startTimeId) {
	return "/programs/" + to_string(programId) + "/starttimes/" + to_string(startTimeId);
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onGetLogs(const HttpRequest& request, const KeyValue& pathParameters) {
	return HttpResponse::Builder().
			setStatus(200, "OK").
			setBody(logWriter->toString(LOGGER.getEntries())).
			addHeader("Content-Type", "application/xml").
			build();
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onPatchIrrigation(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not start or stop irrigation";

	try {
		const IrrigationActionDTO irrigationActionDTO = XmlIrrigationActionReader().load(string(request.getUploadData()->data(), request.getUploadData()->size()));

		if (irrigationActionDTO.action.get() == nullptr) {
			const char* message = "The 'action' element tag not found";
			LOGGER.warning(message);
			throw RestBadRequest(restService->getErrorWriter(), message);
		}

		const string action = *irrigationActionDTO.action.get();

		if (action == "start") {
			onPatchIrrigation_startCustom(irrigationActionDTO);
		} else if (action == "start-program") {
			onPatchIrrigation_startProgram(irrigationActionDTO);
		} else if (action == "stop") {
			onPatchIrrigation_stop(irrigationActionDTO);
		} else {
			const string message = "Invalid value of 'action' element: " + action;
			LOGGER.warning(message.c_str());
			throw RestBadRequest(restService->getErrorWriter(), message);
		}

		return HttpResponse::Builder().
				setStatus(200, "OK").
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

void RestView::onPatchIrrigation_startCustom(const IrrigationActionDTO& irrigationActionDTO) {
	if (irrigationActionDTO.runTimeDtoList.get() == nullptr) {
		const char* message = "The 'runtimes' element tag not found";
		LOGGER.warning(message);
		throw RestBadRequest(restService->getErrorWriter(), message);
	}

	unique_lock<IrrigationDocument> lock(irrigationDocument);

	irrigationDocument.getWateringController().start(
		RunTimeContainer(*irrigationActionDTO.runTimeDtoList),
		irrigationActionDTO.adjustment.get() ? *irrigationActionDTO.adjustment : 100
	);
}

void RestView::onPatchIrrigation_startProgram(const IrrigationActionDTO& irrigationActionDTO) {
	if (irrigationActionDTO.programId.get() == nullptr) {
		const char* message = "The 'program-id' element tag not found";
		LOGGER.warning(message);
		throw RestBadRequest(restService->getErrorWriter(), message);
	}

	unique_lock<IrrigationDocument> lock(irrigationDocument);

	const IdType programId = *irrigationActionDTO.programId;
	const shared_ptr<Program> program = irrigationDocument.getPrograms().at(programId);

	irrigationDocument.getWateringController().start(
		program->getRunTimes(),
		irrigationActionDTO.adjustment.get() ? *irrigationActionDTO.adjustment : program->getAdjustment()
	);
}

void RestView::onPatchIrrigation_stop(const IrrigationActionDTO& irrigationActionDTO) {
	unique_lock<IrrigationDocument> lock(irrigationDocument);
	irrigationDocument.getWateringController().stop();
}

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<HttpResponse> RestView::onPatchReboot(const HttpRequest& request, const KeyValue& pathParameters) {
	const int systemExitCode = std::system("shutdown --reboot now");
	if (0 != systemExitCode) {
		const int processExitCode = (systemExitCode & 0xFF00) >> 8;
		throw RestInternalServerError(restService->getErrorWriter(), "Can not execute reboot. Error: " + std::to_string(processExitCode));
	}

	return HttpResponse::Builder().
			setStatus(200, "OK").
			build();
}

std::unique_ptr<HttpResponse> RestView::onPatchPoweroff(const HttpRequest& request, const KeyValue& pathParameters) {
	const int systemExitCode = std::system("shutdown --poweroff now");
	if (0 != systemExitCode) {
		const int processExitCode = (systemExitCode & 0xFF00) >> 8;
		throw RestInternalServerError(restService->getErrorWriter(), "Can not execute power off. Error: " + std::to_string(processExitCode));
	}

	return HttpResponse::Builder().
			setStatus(200, "OK").
			build();
}
