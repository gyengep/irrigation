#include "RestView.h"
#include "RestService.h"
#include "RestServiceException.h"
#include "XmlTemperatureWriter.h"
#include "Logger/Logger.h"
#include "Temperature/CurrentTemperature.h"
#include "Temperature/TemperatureException.h"
#include "Utils/DateTime.h"

using namespace std;


unique_ptr<HttpResponse> RestView::onGetTemperature(const HttpRequest& request, const KeyValue& pathParameters) {
	try {
		const float temperature = currentTemperature->getCurrentTemperature();

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(temperatureWriter->currentToString(temperature)).
				addHeader("Content-Type", temperatureWriter->contentType()).
				build();

	} catch (const TemperatureException& e) {
		LOGGER.warning("Can not retrieve temperature", e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}

std::unique_ptr<HttpResponse> RestView::onGetTemperatureForecast(const HttpRequest& request, const KeyValue& pathParameters) {
	try {
		int daysToAdd = 0;

		const auto it = request.getParameters().find("day");
		if (request.getParameters().end() != it) {
			const std::string day = it->second;

			if (day == "today") {
				daysToAdd = 0;
			} else if (day == "tomorrow") {
				daysToAdd = 1;
			} else {
				const string message = "Invalid day: " + day;
				LOGGER.warning(message.c_str());
				throw RestBadRequest(restService->getErrorWriter(), message);
			}
		}

		const LocalDateTime dateTime = LocalDateTime::now().addDays(daysToAdd);
		const LocalDateTime from = LocalDateTime::create(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 0, 0, 0);
		const LocalDateTime to = LocalDateTime::create(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 23, 59, 59);
		const auto forecastValues = temperatureForecast->getTemperatureForecast(from.toRawtime(), to.toRawtime());

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(temperatureWriter->forecastToString(forecastValues, from, to)).
				addHeader("Content-Type", temperatureWriter->contentType()).
				build();

	} catch (const TemperatureException& e) {
		LOGGER.warning("Can not retrieve temperature forecast", e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}

std::unique_ptr<HttpResponse> RestView::onGetTemperatureHistory(const HttpRequest& request, const KeyValue& pathParameters) {
	try {

		int daysToAdd = 0;

		const auto it = request.getParameters().find("day");
		if (request.getParameters().end() != it) {
			const std::string day = it->second;

			if (day == "today") {
				daysToAdd = 0;
			} else if (day == "yesterday") {
				daysToAdd = -1;
			} else {
				const string message = "Invalid day: " + day;
				LOGGER.warning(message.c_str());
				throw RestBadRequest(restService->getErrorWriter(), message);
			}
		}

		const LocalDateTime dateTime = LocalDateTime::now().addDays(daysToAdd);
		const LocalDateTime from = LocalDateTime::create(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 0, 0, 0);
		const LocalDateTime to = LocalDateTime::create(dateTime.getYears(), dateTime.getMonths(), dateTime.getDays(), 23, 59, 59);
		const auto historyValues = temperatureHistory->getTemperatureHistory(from.toRawtime(), to.toRawtime());

		return HttpResponse::Builder().
				setStatus(200, "OK").
				setBody(temperatureWriter->historyToString(historyValues, from, to)).
				addHeader("Content-Type", temperatureWriter->contentType()).
				build();

	} catch (const TemperatureException& e) {
		LOGGER.warning("Can not retrieve temperature history", e);
		throw RestNotFound(restService->getErrorWriter(), e.what());
	}
}
