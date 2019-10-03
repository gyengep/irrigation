#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/TemperatureDependentSchedulerDTO.h"

#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_1 temperatureDependentSchedulerSample_all
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_2 temperatureDependentSchedulerSample_remainingA
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_3 temperatureDependentSchedulerSample_forecastA
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_4 temperatureDependentSchedulerSample_forecastB
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_5 temperatureDependentSchedulerSample_historyA
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_6 temperatureDependentSchedulerSample_historyB
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_7 temperatureDependentSchedulerSample_minAdjustment
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_8 temperatureDependentSchedulerSample_maxAdjustment
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_9 temperatureDependentSchedulerSample_trim
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_10 temperatureDependentSchedulerSample_empty

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, TemperatureDependentSchedulerDTO> TemperatureDependentSchedulerSample;

///////////////////////////////////////////////////////////////////////////////

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_all(
		"<scheduler type=\"temperature-dependent\">"
			"<remaining-a>2.5</remaining-a>"
			"<forecast-a>3.5</forecast-a>"
			"<forecast-b>4.5</forecast-b>"
			"<history-a>5.5</history-a>"
			"<history-b>6.5</history-b>"
			"<min-adjustment>85</min-adjustment>"
			"<max-adjustment>86</max-adjustment>"
			"<trim>87</trim>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO(2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 85, 86, 87)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_remainingA(
		"<scheduler type=\"temperature-dependent\">"
			"<remaining-a>12.5</remaining-a>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setRemainingA(12.5f)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_forecastA(
		"<scheduler type=\"temperature-dependent\">"
			"<forecast-a>13.5</forecast-a>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setForecastA(13.5f)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_forecastB(
		"<scheduler type=\"temperature-dependent\">"
			"<forecast-b>14.5</forecast-b>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setForecastB(14.5f)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_historyA(
		"<scheduler type=\"temperature-dependent\">"
			"<history-a>15.5</history-a>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setHistoryA(15.5f)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_historyB(
		"<scheduler type=\"temperature-dependent\">"
			"<history-b>16.5</history-b>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setHistoryB(16.5f)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_minAdjustment(
		"<scheduler type=\"temperature-dependent\">"
			"<min-adjustment>95</min-adjustment>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setMinAdjustment(95)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_maxAdjustment(
		"<scheduler type=\"temperature-dependent\">"
			"<max-adjustment>96</max-adjustment>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setMaxAdjustment(96)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_trim(
		"<scheduler type=\"temperature-dependent\">"
			"<trim>97</trim>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setTrim(97)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_empty(
		"<scheduler type=\"temperature-dependent\"/>",
		TemperatureDependentSchedulerDTO()
		);
