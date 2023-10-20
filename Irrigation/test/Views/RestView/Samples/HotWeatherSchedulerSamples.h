#pragma once
#include <memory>
#include <vector>
#include "DTO/HotWeatherSchedulerDto.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	typedef Sample<HotWeatherSchedulerDto> HotWeatherSchedulerSample;

	class HotWeatherSchedulerSampleList : public std::vector<HotWeatherSchedulerSample> {
	public:
		HotWeatherSchedulerSampleList() {

			emplace_back(
				HotWeatherSchedulerDto(60, 35.0f),
				"<scheduler type=\"hot-weather\">"
					"<period>60</period>"
					"<temperature>35</temperature>"
				"</scheduler>"
			);

			emplace_back(
				HotWeatherSchedulerDto(120, 30.0f),
				"<scheduler type=\"hot-weather\">"
					"<period>120</period>"
					"<temperature>30</temperature>"
				"</scheduler>"
			);

			emplace_back(
				HotWeatherSchedulerDto(180, 33.0f),
				"<scheduler type=\"hot-weather\">"
					"<period>180</period>"
					"<temperature>33</temperature>"
				"</scheduler>"
			);

			emplace_back(
				HotWeatherSchedulerDto(150, 25.0f),
				"<scheduler type=\"hot-weather\">"
					"<period>150</period>"
					"<temperature>25</temperature>"
				"</scheduler>"
			);
		}
	};
};
