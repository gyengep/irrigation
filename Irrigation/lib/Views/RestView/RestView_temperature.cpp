#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
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

unique_ptr<HttpResponse> RestView::onGetCurrentTemperature(const HttpRequest& request, const KeyValue& pathParameters) {
	try {
		const std::string datetimeFormat = getDatetimeFormatParameter(request.getParameters());
		const auto dto = currentTemperature->toCurrentTemperatureDto(datetimeFormat);

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(dto, "/temperature-current.xsl")).
				addHeader("Content-Type", dtoWriter->getContentType()).
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
		const LocalDateTime from(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 0, 0, 0);
		const LocalDateTime to(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 23, 59, 59);

		const auto temperatureHistoryDto = temperatureHistory->toTemperatureHistoryDto(from, to, datetimeFormat);

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(temperatureHistoryDto, "/temperature-yesterday.xsl")).
				addHeader("Content-Type", dtoWriter->getContentType()).
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
		const LocalDateTime from(now.getYears(), now.getMonths(), now.getDays(), 0, 0, 0);
		const LocalDateTime to(now.getYears(), now.getMonths(), now.getDays(), 23, 59, 59);

		const auto temperatureHistoryDto = temperatureHistory->toTemperatureHistoryDto(from, now, datetimeFormat);
		const auto temperatureForecastDto = temperatureForecast->toTemperatureForecastDto(now, to, datetimeFormat);

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(temperatureHistoryDto, temperatureForecastDto, "/temperature-today.xsl")).
				addHeader("Content-Type", dtoWriter->getContentType()).
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
		const LocalDateTime from(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 0, 0, 0);
		const LocalDateTime to(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 23, 59, 59);

		const auto temperatureForecastDto = temperatureForecast->toTemperatureForecastDto(from, to, datetimeFormat);

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(dtoWriter->save(temperatureForecastDto, "/temperature-tomorrow.xsl")).
				addHeader("Content-Type", dtoWriter->getContentType()).
				build();

	} catch (const TemperatureException& e) {
		LOGGER.warning("Can not retrieve temperature", e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}
