#pragma once
#include <string>
#include <vector>
#include "DTO/HotWeatherSchedulerDto.h"
#include "SampleBase.h"


namespace DtoReaderWriterTestSamples {

	typedef Sample<HotWeatherSchedulerDto> HotWeatherSchedulerSample;

	class HotWeatherSchedulerSampleList : public std::vector<HotWeatherSchedulerSample> {
	public:
		HotWeatherSchedulerSampleList() {

			emplace_back(
				HotWeatherSchedulerDto(),
				"<scheduler type=\"hot-weather\"/>"
			);

			emplace_back(
				HotWeatherSchedulerDto().setPeriodInSeconds(155),
				"<scheduler type=\"hot-weather\">"
					"<period>155</period>"
				"</scheduler>"
			);

			emplace_back(
				HotWeatherSchedulerDto().setMinTemperature(24),
				"<scheduler type=\"hot-weather\">"
					"<temperature>24</temperature>"
				"</scheduler>"
			);

			emplace_back(
				HotWeatherSchedulerDto(145, 26.0f),
				"<scheduler type=\"hot-weather\">"
					"<period>145</period>"
					"<temperature>26</temperature>"
				"</scheduler>"
			);
		}
	};
};
