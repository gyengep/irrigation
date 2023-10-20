#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "XmlErrorWriter.h"
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
#include <cstring>
#include <fstream>

using namespace std;


RestView::RestView(IrrigationDocument& irrigationDocument, uint16_t port,
		const std::shared_ptr<CurrentTemperature>& currentTemperature,
		const std::shared_ptr<TemperatureForecast>& temperatureForecast,
		const std::shared_ptr<TemperatureHistory>& temperatureHistory,
		const std::shared_ptr<ShutdownManager>& shutdownManager,
		const std::shared_ptr<FileWriterFactory>& accessLogWriterFactory,
		const std::string& resourceDirectory
) :
	View(irrigationDocument),
	port(port),
	currentTemperature(currentTemperature),
	temperatureForecast(temperatureForecast),
	temperatureHistory(temperatureHistory),
	shutdownManager(shutdownManager),
	resourceDirectory(resourceDirectory),
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

	if (nullptr == shutdownManager) {
		throw std::invalid_argument("RestView::RestView() nullptr == shutdownManager");
	}

	restService.reset(new RestService());
	webServer.reset(new WebServer(restService, port, accessLogWriterFactory));
	dtoReader.reset(new XmlReader());
	dtoWriter.reset(new XmlWriter());

	using namespace placeholders;

	restService->addPath(MHD_HTTP_METHOD_GET,  	 "/api/v1/programs", bind(&RestView::onGetProgramList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_POST,   "/api/v1/programs", bind(&RestView::onPostProgramList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/api/v1/programs/{programId}", bind(&RestView::onGetProgram, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/api/v1/programs/{programId}", bind(&RestView::onPatchProgram, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_DELETE, "/api/v1/programs/{programId}", bind(&RestView::onDeleteProgram, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/api/v1/programs/{programId}/runtimes", bind(&RestView::onGetRunTimeList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/api/v1/programs/{programId}/runtimes", bind(&RestView::onPatchRunTimeList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/api/v1/programs/{programId}/starttimes", bind(&RestView::onGetStartTimeList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_POST, 	 "/api/v1/programs/{programId}/starttimes", bind(&RestView::onPostStartTimeList, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,  	 "/api/v1/programs/{programId}/starttimes/{startTimeId}", bind(&RestView::onGetStartTime, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/api/v1/programs/{programId}/starttimes/{startTimeId}", bind(&RestView::onPatchStartTime, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_DELETE, "/api/v1/programs/{programId}/starttimes/{startTimeId}", bind(&RestView::onDeleteStartTime, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/api/v1/programs/{programId}/schedulers/hot-weather", bind(&RestView::onGetHotWeatherScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/api/v1/programs/{programId}/schedulers/hot-weather", bind(&RestView::onPatchHotWeatherScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/api/v1/programs/{programId}/schedulers/temperature-dependent", bind(&RestView::onGetTemperatureDependentScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/api/v1/programs/{programId}/schedulers/temperature-dependent", bind(&RestView::onPatchTemperatureDependentScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/api/v1/programs/{programId}/schedulers/weekly", bind(&RestView::onGetWeeklyScheduler, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/api/v1/programs/{programId}/schedulers/weekly", bind(&RestView::onPatchWeeklyScheduler, this, _1, _2));

	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/api/v1/irrigation", bind(&RestView::onPatchIrrigation, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/api/v1/power/shutdown", bind(&RestView::onPatchShutdown, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/api/v1/power/reboot", bind(&RestView::onPatchReboot, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/api/v1/logs", bind(&RestView::onGetLogs, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/api/v1/temperature/current", bind(&RestView::onGetCurrentTemperature, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/api/v1/temperature/yesterday", bind(&RestView::onGetTemperatureYesterday, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/api/v1/temperature/today", bind(&RestView::onGetTemperatureToday, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/api/v1/temperature/tomorrow", bind(&RestView::onGetTemperatureTomorrow, this, _1, _2));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/{fileName}", bind(&RestView::onGetFile, this, _1, _2, ""));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/css/{fileName}", bind(&RestView::onGetFile, this, _1, _2, "/css"));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/images/{fileName}", bind(&RestView::onGetFile, this, _1, _2, "/images"));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/scripts/{fileName}", bind(&RestView::onGetFile, this, _1, _2, "/scripts"));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/", bind(&RestView::onGetRoot, this, _1, _2));
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
	return "/programs/" + programId.toString();
}

string RestView::getStartTimeUrl(const IdType& programId, const IdType& startTimeId) {
	return "/programs/" + programId.toString() + "/starttimes/" + startTimeId.toString();
}

///////////////////////////////////////////////////////////////////////////////

std::string getFileExtension(const std::string& fileName) {
	const auto pos = fileName.find_last_of(".");
	std::string result;

	if (std::string::npos != pos) {
		result = fileName.substr(pos + 1);
	}

	return result;
}

std::string getContentTypeForFile(const std::string& fileName) {
	const auto extension = getFileExtension(fileName);

	std::string result;

	if (extension == "js") {
		result = "text/javascript";
	} else if (extension == "ico") {
		result = "image/x-icon";
	} else if (extension == "png") {
		result = "image/png";
	} else {
		result = "text/" + extension;
	}

	return result;
}

std::unique_ptr<HttpResponse> RestView::getFile(const std::string fileName) {
	try {
		std::ifstream file(fileName);

		if (file.fail()) {
			LOGGER.warning("File not found: %s", fileName.c_str());
			throw FileNotFoundException();
		}

		std::string str((std::istreambuf_iterator<char>(file)),
						 std::istreambuf_iterator<char>());

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(str).
				addHeader("Content-Type", getContentTypeForFile(fileName)).
				build();

	} catch (const FileNotFoundException& e) {
		LOGGER.warning("File not found", e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}

unique_ptr<HttpResponse> RestView::onGetRoot(const HttpRequest& request, const KeyValue& pathParameters) {
	const std::string fileName = resourceDirectory + "/index.html";
	return getFile(fileName);
}

unique_ptr<HttpResponse> RestView::onGetFile(const HttpRequest& request, const KeyValue& pathParameters, const std::string& subDirectory) {
	const std::string fileName = resourceDirectory + subDirectory + "/" + pathParameters.at("fileName");
	return getFile(fileName);
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onGetLogs(const HttpRequest& request, const KeyValue& pathParameters) {
	return HttpResponse::Builder().
			setStatus(200, "OK").
			setBody(dtoWriter->save(LOGGER.getEntries(), "/logs.xsl")).
			addHeader("Content-Type", dtoWriter->getContentType()).
			build();
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onPatchIrrigation(const HttpRequest& request, const KeyValue& pathParameters) {
	static const char* logMessage = "Can not start or stop irrigation";

	try {
		const IrrigationActionDto irrigationActionDto = XmlIrrigationActionReader().load(string(request.getUploadData()->data(), request.getUploadData()->size()));

		if (irrigationActionDto.action.get() == nullptr) {
			const char* message = "The 'action' element tag not found";
			LOGGER.warning(message);
			throw RestBadRequest(restService->getErrorWriter(), message);
		}

		const string action = *irrigationActionDto.action.get();

		if (action == "start") {
			onPatchIrrigation_startCustom(irrigationActionDto);
		} else if (action == "start-program") {
			onPatchIrrigation_startProgram(irrigationActionDto);
		} else if (action == "stop") {
			onPatchIrrigation_stop(irrigationActionDto);
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

void RestView::onPatchIrrigation_startCustom(const IrrigationActionDto& irrigationActionDto) {
	if (irrigationActionDto.runTimeDtoList.get() == nullptr) {
		const char* message = "The 'runtimes' element tag not found";
		LOGGER.warning(message);
		throw RestBadRequest(restService->getErrorWriter(), message);
	}

	unique_lock<IrrigationDocument> lock(irrigationDocument);

	irrigationDocument.startCustom(
		RunTimeContainer::toDurationList(*irrigationActionDto.runTimeDtoList),
		irrigationActionDto.adjustment.get() ? *irrigationActionDto.adjustment : 100
	);
}

void RestView::onPatchIrrigation_startProgram(const IrrigationActionDto& irrigationActionDto) {
	if (irrigationActionDto.programId.get() == nullptr) {
		const char* message = "The 'program-id' element tag not found";
		LOGGER.warning(message);
		throw RestBadRequest(restService->getErrorWriter(), message);
	}

	unique_lock<IrrigationDocument> lock(irrigationDocument);

	const IdType programId = *irrigationActionDto.programId;

	irrigationDocument.startProgram(
		programId,
		irrigationActionDto.adjustment.get() ? *irrigationActionDto.adjustment : 100
	);
}

void RestView::onPatchIrrigation_stop(const IrrigationActionDto& irrigationActionDto) {
	unique_lock<IrrigationDocument> lock(irrigationDocument);
	irrigationDocument.stop();
}

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<HttpResponse> RestView::onPatchReboot(const HttpRequest& request, const KeyValue& pathParameters) {
	try {
		shutdownManager->reboot();
	} catch(const std::exception& e) {
		throw RestInternalServerError(restService->getErrorWriter(), std::string("Can not execute reboot. Error: ") + e.what());
	}

	return HttpResponse::Builder().
			setStatus(204, "No Content").
			build();
}

std::unique_ptr<HttpResponse> RestView::onPatchShutdown(const HttpRequest& request, const KeyValue& pathParameters) {
	try {
		shutdownManager->powerOff();
	} catch(const std::exception& e) {
		throw RestInternalServerError(restService->getErrorWriter(), std::string("Can not execute power off. Error: ") + e.what());
	}

	return HttpResponse::Builder().
			setStatus(204, "No Content").
			build();
}
