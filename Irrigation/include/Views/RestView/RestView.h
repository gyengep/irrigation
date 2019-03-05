#pragma once
#include <memory>
#include "DocumentView/View.h"
#include "DtoReaderWriter/DtoReaderWriter.h"
#include "Model/IrrigationDocument.h"
#include "WebServer/ErrorWriter.h"
#include "WebServer/RestServiceException.h"


class Program;
class RunTime;
class StartTime;
class RestService;
class WebServer;
class HttpResponse;
class HttpRequest;


class RestView : public View {
	class RestReaderWriterFactory {
		const std::unique_ptr<DtoReaderWriterFactory> dtoReaderWriterFactory;
		const std::unique_ptr<ErrorWriterFactory> errorWriterFactory;
		const std::string contentType;

	public:
		RestReaderWriterFactory(
			std::unique_ptr<DtoReaderWriterFactory>&& dtoReaderWriterFactory,
			std::unique_ptr<ErrorWriterFactory>&& errorWriterFactory,
			const std::string& contentType);
		~RestReaderWriterFactory() = default;

		std::unique_ptr<DtoWriter> createDtoWriter() const { return dtoReaderWriterFactory->createDtoWriter(); }
		std::unique_ptr<DtoReader> createDtoReader() const { return dtoReaderWriterFactory->createDtoReader(); }
		std::unique_ptr<ErrorWriter> createErrorWriter() const { return errorWriterFactory->create(); }
		const std::string& getContentType() const { return contentType; }
	};

	const uint16_t port;

	IrrigationDocument& irrigationDocument;
	std::shared_ptr<RestService> restService;
	std::unique_ptr<WebServer> webServer;
	RestReaderWriterFactory xmlRestReaderWriterFactory;
	RestReaderWriterFactory textRestReaderWriterFactory;

	std::unique_ptr<HttpResponse> onGetProgram(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory);
	std::unique_ptr<HttpResponse> onGetProgramList(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory);
	std::unique_ptr<HttpResponse> onGetStartTime(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory);
	std::unique_ptr<HttpResponse> onGetStartTimeList(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory);
	std::unique_ptr<HttpResponse> onGetRunTime(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory);
	std::unique_ptr<HttpResponse> onGetRunTimeList(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory);

	std::unique_ptr<HttpResponse> onPostStartTime(const HttpRequest& request, const KeyValue& pathParameters, const RestReaderWriterFactory& restReaderWriterFactory);

	const std::shared_ptr<Program>& getProgram(const std::string& programIdText) const;
	const std::shared_ptr<RunTime>& getRunTime(const std::string& programIdText, const std::string& runTimeIdText) const;
	const std::shared_ptr<StartTime>& getStartTime(const std::string& programIdText, const std::string& startTimeIdText) const;

public:
	RestView(IrrigationDocument& irrigationDocument, uint16_t port);
	virtual ~RestView();

	virtual void initialize() override;
	virtual void terminate() override;
};
