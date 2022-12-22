#pragma once
#include <string>
#include <vector>
#include "DTO/HotWeatherSchedulerDTO.h"
#include "SampleBase.h"


namespace DtoReaderWriterTestSamples {

	typedef Sample<HotWeatherSchedulerDTO> HotWeatherSchedulerSample;

	class HotWeatherSchedulerSampleList : public std::vector<HotWeatherSchedulerSample> {
	public:
		HotWeatherSchedulerSampleList() {

			emplace_back(
				HotWeatherSchedulerDTO(),
				"<scheduler type=\"hot-weather\"/>"
			);

			emplace_back(
				HotWeatherSchedulerDTO().setPeriodInSeconds(155),
				"<scheduler type=\"hot-weather\">"
					"<period>155</period>"
				"</scheduler>"
			);

			emplace_back(
				HotWeatherSchedulerDTO().setMinTemperature(24),
				"<scheduler type=\"hot-weather\">"
					"<temperature>24</temperature>"
				"</scheduler>"
			);

			emplace_back(
				HotWeatherSchedulerDTO(145, 26.0f),
				"<scheduler type=\"hot-weather\">"
					"<period>145</period>"
					"<temperature>26</temperature>"
				"</scheduler>"
			);
		}
	};
};
