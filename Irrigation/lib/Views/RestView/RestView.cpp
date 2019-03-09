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

	restService->addPath(MHD_HTTP_METHOD_GET,  	 "/programs", bind(&RestView::onGetProgramList, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_POST,   "/programs", bind(&RestView::onPostProgramList, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/programs/{programId}", bind(&RestView::onGetProgram, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/programs/{programId}", bind(&RestView::onPatchProgram, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_DELETE, "/programs/{programId}", bind(&RestView::onDeleteProgram, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/programs/{programId}/runtimes", bind(&RestView::onGetRunTimeList, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET,  	 "/programs/{programId}/runtimes/{runTimeId}", bind(&RestView::onGetRunTime, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/programs/{programId}/runtimes/{runTimeId}", bind(&RestView::onPatchRunTime, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET,    "/programs/{programId}/starttimes", bind(&RestView::onGetStartTimeList, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_POST, 	 "/programs/{programId}/starttimes", bind(&RestView::onPostStartTimeList, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_GET,  	 "/programs/{programId}/starttimes/{startTimeId}", bind(&RestView::onGetStartTime, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_PATCH,  "/programs/{programId}/starttimes/{startTimeId}", bind(&RestView::onPatchStartTime, this, _1, _2, cref(xmlRestReaderWriterFactory)));
	restService->addPath(MHD_HTTP_METHOD_DELETE,  "/programs/{programId}/starttimes/{startTimeId}", bind(&RestView::onDeleteStartTime, this, _1, _2, cref(xmlRestReaderWriterFactory)));
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
	return programs.at(IdType::from_string(programIdText));
}

const shared_ptr<RunTime>& RestView::getRunTime(const string& programIdText, const string& runTimeIdText) const {
	const RunTimeContainer& runTimes = getProgram(programIdText)->getRunTimes();
	return runTimes.at(IdType::from_string(runTimeIdText));
}
const shared_ptr<StartTime>& RestView::getStartTime(const string& programIdText, const string& startTimeIdText) const {
	const StartTimeContainer& startTimes = getProgram(programIdText)->getStartTimes();
	return startTimes.at(stoul(startTimeIdText));
}

string RestView::getProgramUrl(const IdType& programId) {
	return "/programs/" + to_string(programId);
}

string RestView::getRunTimeUrl(const IdType& programId, const IdType& runTimeId) {
	return "/programs/" + to_string(programId) + "/runtimes/" + to_string(runTimeId);
}

string RestView::getStartTimeUrl(const IdType& programId, const IdType& startTimeId) {
	return "/programs/" + to_string(programId) + "/starttimes/" + to_string(startTimeId);
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onGetProgramList(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
	const list<ProgramDTO> programDtoList = irrigationDocument.getPrograms().toProgramDtoList();
	const string text = dtoWriter->save(programDtoList);

	return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
}

unique_ptr<HttpResponse> RestView::onGetRunTimeList(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
		const list<RunTimeDTO> runTimeDtoList = getProgram(pathParameters.at("programId"))->getRunTimes().toRunTimeDtoList();
		const string text = dtoWriter->save(runTimeDtoList);

		return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onGetStartTimeList(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
		const list<StartTimeDTO> startTimeDtoList = getProgram(pathParameters.at("programId"))->getStartTimes().toStartTimeDtoList();
		const string text = dtoWriter->save(startTimeDtoList);

		return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onGetProgram(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
		const ProgramDTO programDto = getProgram(pathParameters.at("programId"))->toProgramDto();
		const string text = dtoWriter->save(programDto);

		return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	} catch (const IllegalArgumentException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onGetRunTime(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
		const RunTimeDTO runTimeDto = getRunTime(pathParameters.at("programId"), pathParameters.at("runTimeId"))->toRunTimeDto();
		const string text = dtoWriter->save(runTimeDto);

		return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onGetStartTime(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const unique_ptr<DtoWriter> dtoWriter = restReaderWriterFactory.createDtoWriter();
		const StartTimeDTO startTimeDto = getStartTime(pathParameters.at("programId"), pathParameters.at("startTimeId"))->toStartTimeDto();
		const string text = dtoWriter->save(startTimeDto);

		return unique_ptr<HttpResponse>(new HttpResponse(text, headers));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onPostProgramList(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const unique_ptr<DtoReader> dtoReader = restReaderWriterFactory.createDtoReader();
		const ProgramDTO programDto = dtoReader->loadProgram(string(request.getUploadData()->data(), request.getUploadData()->size()));
		const shared_ptr<Program> program(new Program());
		program->updateFromProgramDto(programDto);

		const IdType programId = irrigationDocument.getPrograms().insert(IdType(), program).first;

		headers.insert(make_pair("Location", getProgramUrl(programId)));
		return unique_ptr<HttpResponse>(new HttpResponse("", headers, 201));
	} catch (const exception& e) {
		throw RestBadRequest(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onPostStartTimeList(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const unique_ptr<DtoReader> dtoReader = restReaderWriterFactory.createDtoReader();
		const StartTimeDTO startTimeDto = dtoReader->loadStartTime(string(request.getUploadData()->data(), request.getUploadData()->size()));
		const shared_ptr<StartTime> startTime(new StartTime());
		startTime->updateFromStartTimeDto(startTimeDto);

		const IdType programId = IdType::from_string(pathParameters.at("programId"));
		const IdType startTimeId = irrigationDocument.getPrograms().at(programId)->getStartTimes().insert(IdType(), startTime).first;

		headers.insert(make_pair("Location", getStartTimeUrl(programId, startTimeId)));
		return unique_ptr<HttpResponse>(new HttpResponse("", headers, 201));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	} catch (const exception& e) {
		throw RestBadRequest(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onPatchProgram(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const unique_ptr<DtoReader> dtoReader = restReaderWriterFactory.createDtoReader();
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const shared_ptr<Program> program = getProgram(pathParameters.at("programId"));
		const ProgramDTO programDto = dtoReader->loadProgram(string(request.getUploadData()->data(), request.getUploadData()->size()));
		program->updateFromProgramDto(programDto);
		return unique_ptr<HttpResponse>(new HttpResponse("No Content", KeyValue(), 204));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	} catch (const exception& e) {
		throw RestBadRequest(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onPatchRunTime(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const unique_ptr<DtoReader> dtoReader = restReaderWriterFactory.createDtoReader();
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const shared_ptr<RunTime> runTime = getRunTime(pathParameters.at("programId"), pathParameters.at("runTimeId"));
		const RunTimeDTO runTimeDto = dtoReader->loadRunTime(string(request.getUploadData()->data(), request.getUploadData()->size()));
		runTime->updateFromRunTimeDto(runTimeDto);
		return unique_ptr<HttpResponse>(new HttpResponse("No Content", KeyValue(), 204));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	} catch (const exception& e) {
		throw RestBadRequest(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onPatchStartTime(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const unique_ptr<DtoReader> dtoReader = restReaderWriterFactory.createDtoReader();
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const shared_ptr<StartTime> startTime = getStartTime(pathParameters.at("programId"), pathParameters.at("startTimeId"));
		const StartTimeDTO startTimeDto = dtoReader->loadStartTime(string(request.getUploadData()->data(), request.getUploadData()->size()));
		startTime->updateFromStartTimeDto(startTimeDto);
		return unique_ptr<HttpResponse>(new HttpResponse("No Content", KeyValue(), 204));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	} catch (const exception& e) {
		throw RestBadRequest(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<HttpResponse> RestView::onDeleteProgram(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		irrigationDocument.getPrograms().erase(IdType::from_string(pathParameters.at("programId")));
		return unique_ptr<HttpResponse>(new HttpResponse("", KeyValue()));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	} catch (const IllegalArgumentException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

unique_ptr<HttpResponse> RestView::onDeleteStartTime(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory) {
	const KeyValue headers ({{"Content-Type", restReaderWriterFactory.getContentType()}});

	try {
		const shared_ptr<Program> program = getProgram(pathParameters.at("programId"));
		program->getStartTimes().erase(IdType::from_string(pathParameters.at("startTimeId")));
		return unique_ptr<HttpResponse>(new HttpResponse("", KeyValue()));
	} catch (const NoSuchElementException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	} catch (const IllegalArgumentException& e) {
		throw RestHttpNotFount(restReaderWriterFactory.createErrorWriter(), e.what(), headers);
	}
}

