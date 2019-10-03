#pragma once
#include <list>
#include <string>
#include <utility>
#include "DTO/HotWeatherSchedulerDTO.h"

#define HOT_WEATHER_SCHEDULER_SAMPLE_1 hotWeatherSchedulerSample_all
#define HOT_WEATHER_SCHEDULER_SAMPLE_2 hotWeatherSchedulerSample_period
#define HOT_WEATHER_SCHEDULER_SAMPLE_3 hotWeatherSchedulerSample_temperature
#define HOT_WEATHER_SCHEDULER_SAMPLE_4 hotWeatherSchedulerSample_empty

///////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, HotWeatherSchedulerDTO> HotWeatherSchedulerSample;

///////////////////////////////////////////////////////////////////////////////

const HotWeatherSchedulerSample hotWeatherSchedulerSample_all(
		"<scheduler type=\"hot-weather\">"
			"<period>145</period>"
			"<temperature>26</temperature>"
		"</scheduler>",
		HotWeatherSchedulerDTO(145, 26.0f)
		);

const HotWeatherSchedulerSample hotWeatherSchedulerSample_period(
		"<scheduler type=\"hot-weather\">"
			"<period>155</period>"
		"</scheduler>",
		HotWeatherSchedulerDTO().setPeriodInSeconds(155)
		);

const HotWeatherSchedulerSample hotWeatherSchedulerSample_temperature(
		"<scheduler type=\"hot-weather\">"
			"<temperature>24</temperature>"
		"</scheduler>",
		HotWeatherSchedulerDTO().setMinTemperature(24)
		);

const HotWeatherSchedulerSample hotWeatherSchedulerSample_empty(
		"<scheduler type=\"hot-weather\"/>",
		HotWeatherSchedulerDTO()
		);
