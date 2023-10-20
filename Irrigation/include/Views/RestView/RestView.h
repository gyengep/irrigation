#pragma once
#include <memory>
#include "DocumentView/View.h"
#include "Temperature/CurrentTemperature.h"
#include "Temperature/TemperatureForecast.h"
#include "Temperature/TemperatureHistory.h"
#include "Utils/FileWriter.h"
#include "Utils/ShutdownManager.h"
#include "WebServer/KeyValue.h"
#include "XmlIrrigationActionReader.h"

class IdType;
class Program;
class StartTime;
class WeeklyScheduler;
class RestService;
class WebServer;
class HttpResponse;
class HttpRequest;
class DtoReader;
class DtoWriter;
class IrrigationDocument;


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
	std::unique_ptr<HttpResponse> onGetTemperatureDependentScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetWeeklyScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchHotWeatherScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchTemperatureDependentScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchWeeklyScheduler(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchIrrigation(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchReboot(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onPatchShutdown(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetLogs(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetCurrentTemperature(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetTemperatureYesterday(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetTemperatureToday(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetTemperatureTomorrow(const HttpRequest& request, const KeyValue& pathParameters);

	std::unique_ptr<HttpResponse> onGetRoot(const HttpRequest& request, const KeyValue& pathParameters);
	std::unique_ptr<HttpResponse> onGetFile(const HttpRequest& request, const KeyValue& pathParameters, const std::string& subDirectory);

	std::unique_ptr<HttpResponse> getFile(const std::string fileName);

	void onPatchIrrigation_startCustom(const IrrigationActionDto& irrigationActionDto);
	void onPatchIrrigation_startProgram(const IrrigationActionDto& irrigationActionDto);
	void onPatchIrrigation_stop(const IrrigationActionDto& irrigationActionDto);

	static IdType getProgramId(const KeyValue& pathParameters);
	static IdType getStartTimeId(const KeyValue& pathParameters);

	static std::string getProgramUrl(const IdType& programId);
	static std::string getStartTimeUrl(const IdType& programId, const IdType& startTimeId);

	static ProgramDtoList getProgramDtoList(IrrigationDocument& irrigationDocument);
	static RunTimeDtoList getRunTimeDtoList(IrrigationDocument& irrigationDocument, const IdType& programId);
	static StartTimeDtoList getStartTimeDtoList(IrrigationDocument& irrigationDocument, const IdType& programId);

	static ProgramDto getProgramDto(IrrigationDocument& irrigationDocument, const IdType& programId);
	static StartTimeDto getStartTimeDto(IrrigationDocument& irrigationDocument, const IdType& programId, const IdType& startTimeId);

	static HotWeatherSchedulerDto           getHotWeatherSchedulerDto(IrrigationDocument& irrigationDocument, const IdType& programId);
	static TemperatureDependentSchedulerDto getTemperatureDependentSchedulerDto(IrrigationDocument& irrigationDocument, const IdType& programId);
	static WeeklySchedulerDto               getWeeklySchedulerDto(IrrigationDocument& irrigationDocument, const IdType& programId);

	static std::string patchProgram(IrrigationDocument& irrigationDocument, const IdType& programId, const ProgramDto& programDto);
	static std::string patchStartTime(IrrigationDocument& irrigationDocument, const IdType& programId, const IdType& startTimeId, const StartTimeDto& startTimeDto);
	static std::string patchHotWeatherScheduler(IrrigationDocument& irrigationDocument, const IdType& programId, const HotWeatherSchedulerDto& hotWeatherSchedulerDto);
	static std::string patchTemperatureDependentScheduler(IrrigationDocument& irrigationDocument, const IdType& programId, const TemperatureDependentSchedulerDto& temperatureDependentSchedulerDto);
	static std::string patchWeeklyScheduler(IrrigationDocument& irrigationDocument, const IdType& programId, const WeeklySchedulerDto& weeklySchedulerDto);
	static std::string patchRunTimeList(IrrigationDocument& irrigationDocument, const IdType& programId, const RunTimeDtoList& runTimeDtoList);

	static void deleteProgram(IrrigationDocument& irrigationDocument, const IdType& programId);
	static void deleteStartTime(IrrigationDocument& irrigationDocument, const IdType& programId, const IdType& startTimeId);

	std::pair<IdType, std::string> postProgramList(IrrigationDocument& irrigationDocument, const ProgramDto& programDto);
	std::pair<IdType, std::string> postStartTimeList(IrrigationDocument& irrigationDocument, const IdType& programId, const StartTimeDto& startTimeDto);

public:
	RestView(IrrigationDocument& irrigationDocument, uint16_t port,
			const std::shared_ptr<CurrentTemperature>& currentTemperature,
			const std::shared_ptr<TemperatureForecast>& temperatureForecast,
			const std::shared_ptr<TemperatureHistory>& temperatureHistory,
			const std::shared_ptr<ShutdownManager>& shutdownManager,
			const std::shared_ptr<FileWriterFactory>& accessLogWriterFactory,
			const std::string& resourceDirectory
		);
	virtual ~RestView();

	virtual void initialize() override;
	virtual void terminate() override;
};
