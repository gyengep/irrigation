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
		"<scheduler type=\"temperature_dependent\">"
			"<remaining_a>2.5</remaining_a>"
			"<forecast_a>3.5</forecast_a>"
			"<forecast_b>4.5</forecast_b>"
			"<history_a>5.5</history_a>"
			"<history_b>6.5</history_b>"
			"<min_adjustment>85</min_adjustment>"
			"<max_adjustment>86</max_adjustment>"
			"<trim>87</trim>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO(2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 85, 86, 87)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_remainingA(
		"<scheduler type=\"temperature_dependent\">"
			"<remaining_a>12.5</remaining_a>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setRemainingA(12.5f)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_forecastA(
		"<scheduler type=\"temperature_dependent\">"
			"<forecast_a>13.5</forecast_a>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setForecastA(13.5f)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_forecastB(
		"<scheduler type=\"temperature_dependent\">"
			"<forecast_b>14.5</forecast_b>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setForecastB(14.5f)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_historyA(
		"<scheduler type=\"temperature_dependent\">"
			"<history_a>15.5</history_a>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setHistoryA(15.5f)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_historyB(
		"<scheduler type=\"temperature_dependent\">"
			"<history_b>16.5</history_b>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setHistoryB(16.5f)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_minAdjustment(
		"<scheduler type=\"temperature_dependent\">"
			"<min_adjustment>95</min_adjustment>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setMinAdjustment(95)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_maxAdjustment(
		"<scheduler type=\"temperature_dependent\">"
			"<max_adjustment>96</max_adjustment>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setMaxAdjustment(96)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_trim(
		"<scheduler type=\"temperature_dependent\">"
			"<trim>97</trim>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setTrim(97)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_empty(
		"<scheduler type=\"temperature_dependent\"/>",
		TemperatureDependentSchedulerDTO()
		);
