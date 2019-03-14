#pragma once
#include <memory>
#include "DocumentView/View.h"
#include "DtoReaderWriter/XmlReaderWriterFactory.h"
#include "Logic/IdType.h"
#include "Model/IrrigationDocument.h"
#include "XmlErrorWriter.h"
#include "RestServiceException.h"


class Program;
class RunTime;
class StartTime;
class RestService;
class WebServer;
class HttpResponse;
class HttpRequest;


class RestView : public View {
	const uint16_t port;

	IrrigationDocument& irrigationDocument;
	std::shared_ptr<RestService> restService;
	std::unique_ptr<WebServer> webServer;
	XmlReaderWriterFactory xmlReaderWriterFactory;
	XmlErrorWriterFactory xmlErrorWriterFactory;

	std::unique_ptr<HttpResponse> onGetProgram(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetRunTime(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetStartTime(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetProgramList(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetRunTimeList(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetStartTimeList(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPostProgramList(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPostStartTimeList(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchProgram(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchRunTime(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchStartTime(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onDeleteProgram(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onDeleteStartTime(const HttpRequest& request, const KeyValue& pathParameters);

	const std::shared_ptr<Program>& getProgram(const std::string& programIdText) const;
	const std::shared_ptr<RunTime>& getRunTime(const std::string& programIdText, const std::string& runTimeIdText) const;
	const std::shared_ptr<StartTime>& getStartTime(const std::string& programIdText, const std::string& startTimeIdText) const;

	static std::string getProgramUrl(const IdType& programId);
	static std::string getRunTimeUrl(const IdType& programId, const IdType& runTimeId);
	static std::string getStartTimeUrl(const IdType& programId, const IdType& startTimeId);

public:
	RestView(IrrigationDocument& irrigationDocument, uint16_t port);
	virtual ~RestView();

	virtual void initialize() override;
	virtual void terminate() override;
};
