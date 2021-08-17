#pragma once
#include <memory>
#include "DocumentView/View.h"
#include "Utils/ShutdownManager.h"
#include "WebServer/KeyValue.h"
#include "XmlIrrigationActionReader.h"

class IdType;
class Program;
class StartTime;
class PeriodicScheduler;
class WeeklyScheduler;
class RestService;
class WebServer;
class HttpResponse;
class HttpRequest;
class DtoReader;
class DtoWriter;
class LogWriter;
class TemperatureWriter;
class IrrigationDocument;
class CurrentTemperature;
class TemperatureForecast;
class TemperatureHistory;


class RestView : public View {
	const uint16_t port;
	const std::shared_ptr<CurrentTemperature> currentTemperature;
	const std::shared_ptr<TemperatureForecast> temperatureForecast;
	const std::shared_ptr<TemperatureHistory> temperatureHistory;
	const std::shared_ptr<ShutdownManager> shutdownManager;
	const std::string resourceDirectory;

	IrrigationDocument& irrigationDocument;

	std::shared_ptr<RestService> restService;
	std::unique_ptr<WebServer> webServer;
	std::shared_ptr<DtoReader> dtoReader;
	std::shared_ptr<DtoWriter> dtoWriter;
	std::shared_ptr<LogWriter> logWriter;
	std::shared_ptr<TemperatureWriter> temperatureWriter;

	std::unique_ptr<HttpResponse> onGetProgram(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetStartTime(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetProgramList(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetRunTimeList(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetStartTimeList(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPostProgramList(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPostStartTimeList(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchProgram(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchRunTimeList(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchStartTime(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onDeleteProgram(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onDeleteStartTime(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetHotWeatherScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetPeriodicScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetTemperatureDependentScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetWeeklyScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchHotWeatherScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchPeriodicScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchTemperatureDependentScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchWeeklyScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchIrrigation(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchReboot(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchPoweroff(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetLogs(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetTemperatureCurrent(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetTemperatureYesterday(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetTemperatureToday(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetTemperatureTomorrow(const HttpRequest& request, const KeyValue& pathParameters);

	std::unique_ptr<HttpResponse> onGetRoot(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetFile(const HttpRequest& request, const KeyValue& pathParameters);

	std::unique_ptr<HttpResponse> getFile(const std::string fileName);

	void onPatchIrrigation_startCustom(const IrrigationActionDTO& irrigationActionDTO);
	void onPatchIrrigation_startProgram(const IrrigationActionDTO& irrigationActionDTO);
	void onPatchIrrigation_stop(const IrrigationActionDTO& irrigationActionDTO);

	static IdType getProgramId(const KeyValue& pathParameters);
	static IdType getStartTimeId(const KeyValue& pathParameters);

	static std::string getProgramUrl(const IdType& programId);
	static std::string getStartTimeUrl(const IdType& programId, const IdType& startTimeId);

public:
	RestView(IrrigationDocument& irrigationDocument, uint16_t port,
			const std::shared_ptr<CurrentTemperature>& currentTemperature,
			const std::shared_ptr<TemperatureForecast>& temperatureForecast,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			const std::shared_ptr<ShutdownManager>& shutdownManager,
			const std::string& resourceDirectory
		);
	virtual ~RestView();

	virtual void initialize() override;
	virtual void terminate() override;
};
