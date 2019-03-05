#include "RestView.h"
#include "TextReaderWriter.h"
#include "XmlReaderWriter.h"
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Model/IrrigationDocument.h"
#include "WebServer/KeyValue.h"
#include "WebServer/RestService.h"

using namespace std;


RestView::RestReaderWriterFactory::RestReaderWriterFactory(
	std::unique_ptr<DtoReaderWriterFactory>&& dtoReaderWriterFactory,
	std::unique_ptr<ErrorWriterFactory>&& errorWriterFactory,
	const std::string& contentType) :

		dtoReaderWriterFactory(move(dtoReaderWriterFactory)),
		errorWriterFactory(move(errorWriterFactory)),
		contentType(contentType)
	{
	}


RestView::RestView(IrrigationDocument& irrigationDocument, uint16_t port) :
	View(irrigationDocument),
	port(port),
	irrigationDocument(irrigationDocument),
	xmlRestReaderWriterFactory(
		unique_ptr<DtoReaderWriterFactory>(new XmlReaderWriterFactory()),
		unique_ptr<ErrorWriterFactory>(new XmlErrorWriterFactory()),
		"application/xml"),
	textRestReaderWriterFactory(
		unique_ptr<DtoReaderWriterFactory>(new TextReaderWriterFactory()),
		unique_ptr<ErrorWriterFactory>(new TextErrorWriterFactory()),
		"text/plain")
{


	//restReaderWriterFactory.reset(new RestReaderWriterFactory());
	restService.reset(new RestService());
	webServer.reset(new WebServer(restService, port));

	using namespace placeholders;

	restService->addPath(MHD_HTTP_METHOD_GET, "/programs", bind(&RestView::onGetProgramList, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET, "/programs/{programId}", bind(&RestView::onGetProgram, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET, "/programs/{programId}/runtimes", bind(&RestView::onGetRunTimeList, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET, "/programs/{programId}/runtimes/{runtimeId}", bind(&RestView::onGetRunTime, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET, "/programs/{programId}/starttimes", bind(&RestView::onGetStartTimeList, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET, "/programs/{programId}/starttimes/{starttimeId}", bind(&RestView::onGetStartTime, this, _1, _2, cref(xmlRestReaderWriterFactory)));

	restService->addPath(MHD_HTTP_METHOD_POST, "/programs/{programId}/starttimes", bind(&RestView::onPostStartTime, this, _1, _2, cref(xmlRestReaderWriterFactory)));

	restService->addPath(MHD_HTTP_METHOD_GET, "/text/programs", bind(&RestView::onGetProgramList, this, _1, _2, cref(textRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET, "/text/programs/{programId}", bind(&RestView::onGetProgram, this, _1, _2, cref(textRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET, "/text/programs/{programId}/runtimes", bind(&RestView::onGetRunTimeList, this, _1, _2, cref(textRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET, "/text/programs/{programId}/runtimes/{runtimeId}", bind(&RestView::onGetRunTime, this, _1, _2, cref(textRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET, "/text/programs/{programId}/starttimes", bind(&RestView::onGetStartTimeList, this, _1, _2, cref(textRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET, "/text/programs/{programId}/starttimes/{starttimeId}", bind(&RestView::onGetStartTime, this, _1, _2, cref(textRestReaderWriterFactory)));
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

const shared_ptr<Program>& RestView::getProgram(const string& programIdText) const {
	const ProgramContainer& programs = irrigationDocument.getPrograms();
	return programs.at(stoul(programIdText));
}

const shared_ptr<RunTime>& RestView::getRunTime(const string& programIdText, const string& runTimeIdText) const {
	const RunTimeContainer& runTimes = getProgram(programIdText)->getRunTimes();
	return runTimes.at(stoul(runTimeIdText));
}
const shared_ptr<StartTime>& RestView::getStartTime(const string& programIdText, const string& startTimeIdText) const {
	const StartTimeContainer& startTimes = getProgram(programIdText)->getStartTimes();
	return startTimes.at(stoul(startTimeIdText));
}

unique_ptr<HttpResponse> RestView::onGetProgramList(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	const string text = dtoWriter->save(irrigationDocument.getPrograms().toProgramDtoList());
	return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
}

unique_ptr<HttpResponse> RestView::onGetRunTimeList(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const string text = dtoWriter->save(getProgram(pathParameters.at("programId"))->getRunTimes().toRunTimeDtoList());
		return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onGetStartTimeList(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const string text = dtoWriter->save(getProgram(pathParameters.at("programId"))->getStartTimes().toStartTimeDtoList());
		return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onGetProgram(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const string text = dtoWriter->save(getProgram(pathParameters.at("programId"))->toProgramDto());
		return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onGetRunTime(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const string text = dtoWriter->save(getRunTime(pathParameters.at("programId"), pathParameters.at("runtimeId"))->toRunTimeDto());
		return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onGetStartTime(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const string text = dtoWriter->save(getStartTime(pathParameters.at("programId"), pathParameters.at("starttimeId"))->toStartTimeDto());
		return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onPostStartTime(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const unique_ptr<DtoReader> dtoReader = restReaderWriterFactory.createDtoReader();
	//const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		shared_ptr<Program> program = getProgram(pathParameters.at("programId"));
		shared_ptr<StartTime> startTime(new StartTime());
		//startTime->updateFromStartTimeDto(request.getUploadData()->data());
		program->getStartTimes().insert(IdType(), startTime);

		return unique_ptr<HttpResponse>(new HttpResponse("Added", KeyValue()));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), KeyValue());
	}
}

