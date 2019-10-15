#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/TemperatureDependentSchedulerDTO.h"

#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_1 temperatureDependentSchedulerSample_all
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_2 temperatureDependentSchedulerSample_remainingCorrection
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_3 temperatureDependentSchedulerSample_minAdjustment
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_4 temperatureDependentSchedulerSample_maxAdjustment
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_5 temperatureDependentSchedulerSample_trim
#define TEMPERATURE_DEPENDENT_SCHEDULER_SAMPLE_6 temperatureDependentSchedulerSample_empty

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, TemperatureDependentSchedulerDTO> TemperatureDependentSchedulerSample;

///////////////////////////////////////////////////////////////////////////////

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_all(
		"<scheduler type=\"temperature-dependent\">"
			"<remaining-correction>2.5</remaining-correction>"
			"<min-adjustment>85</min-adjustment>"
			"<max-adjustment>86</max-adjustment>"
			"<trim>87</trim>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO(2.5f, 85, 86, 87)
		);

const TemperatureDependentSchedulerSample temperatureDependentSchedulerSample_remainingCorrection(
		"<scheduler type=\"temperature-dependent\">"
			"<remaining-correction>12.5</remaining-correction>"
		"</scheduler>",
		TemperatureDependentSchedulerDTO().setRemainingCorrection(12.5f)
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
