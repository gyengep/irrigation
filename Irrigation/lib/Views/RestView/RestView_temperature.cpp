#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "XmlTemperatureWriter.h"
#include "Logger/Logger.h"
#include "Temperature/CurrentTemperature.h"
#include "Temperature/TemperatureException.h"
#include "Utils/DateTime.h"

using namespace std;

std::string getDatetimeFormatParameter(const KeyValue& parameters) {
	const auto it = parameters.find("datetime-format");

	if (parameters.end() == it) {
		return "%a, %d %b %G %H:%M:%S %z";
	}

	return it->second;
}

unique_ptr<HttpResponse> RestView::onGetTemperatureCurrent(const HttpRequest& request, const KeyValue& pathParameters) {
	try {
		const float temperature = currentTemperature->getCurrentTemperature();
		const std::string datetimeFormat = getDatetimeFormatParameter(request.getParameters());

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(temperatureWriter->currentToString(LocalDateTime::now().toString(datetimeFormat), temperature)).
				addHeader("Content-Type", temperatureWriter->contentType()).
				build();

	} catch (const TemperatureException& e) {
		LOGGER.warning("Can not retrieve temperature", e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}

std::unique_ptr<HttpResponse> RestView::onGetTemperatureYesterday(const HttpRequest& request, const KeyValue& pathParameters) {
	try {
		const std::string datetimeFormat = getDatetimeFormatParameter(request.getParameters());

		const LocalDateTime dateTime = LocalDateTime::now().addDays(-1);
		const LocalDateTime from = LocalDateTime::create(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 0, 0, 0);
		const LocalDateTime to = LocalDateTime::create(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 23, 59, 59);
		const auto historyValues = temperatureHistory->getTemperatureHistory(from, to);

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(temperatureWriter->yesterdayToString(from.toString(datetimeFormat), to.toString(datetimeFormat), historyValues)).
				addHeader("Content-Type", temperatureWriter->contentType()).
				build();

	} catch (const TemperatureException& e) {
		LOGGER.warning("Can not retrieve temperature", e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}

std::unique_ptr<HttpResponse> RestView::onGetTemperatureToday(const HttpRequest& request, const KeyValue& pathParameters) {
	try {
		const std::string datetimeFormat = getDatetimeFormatParameter(request.getParameters());

		const LocalDateTime now = LocalDateTime::now();
		const LocalDateTime from = LocalDateTime::create(now.getYears(), now.getMonths(), now.getDays(), 0, 0, 0);
		const LocalDateTime to = LocalDateTime::create(now.getYears(), now.getMonths(), now.getDays(), 23, 59, 59);

		const auto historyValues = temperatureHistory->getTemperatureHistory(from, now);
		const auto forecastValues = temperatureForecast->getTemperatureForecast(now, to);

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(temperatureWriter->todayToString(
						from.toString(datetimeFormat),
						now.toString(datetimeFormat),
						to.toString(datetimeFormat),
						historyValues,
						forecastValues)).
				addHeader("Content-Type", temperatureWriter->contentType()).
				build();

	} catch (const TemperatureException& e) {
		LOGGER.warning("Can not retrieve temperature", e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}

std::unique_ptr<HttpResponse> RestView::onGetTemperatureTomorrow(const HttpRequest& request, const KeyValue& pathParameters) {
	try {
		const std::string datetimeFormat = getDatetimeFormatParameter(request.getParameters());

		const LocalDateTime dateTime = LocalDateTime::now().addDays(1);
		const LocalDateTime from = LocalDateTime::create(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 0, 0, 0);
		const LocalDateTime to = LocalDateTime::create(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 23, 59, 59);
		const auto forecastValues = temperatureForecast->getTemperatureForecast(from, to);

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(temperatureWriter->tomorrowToString(from.toString(datetimeFormat), to.toString(datetimeFormat), forecastValues)).
				addHeader("Content-Type", temperatureWriter->contentType()).
				build();

	} catch (const TemperatureException& e) {
		LOGGER.warning("Can not retrieve temperature", e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}
