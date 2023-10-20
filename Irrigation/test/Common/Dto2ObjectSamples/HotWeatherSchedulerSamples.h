#pragma once
#include <memory>
#include <vector>
#include "DTO/HotWeatherSchedulerDto.h"
#include "Schedulers/Impl/HotWeatherSchedulerImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTestSamples {

	typedef ObjectSample<HotWeatherSchedulerImpl, HotWeatherSchedulerDto> HotWeatherSchedulerSample;

	class HotWeatherSchedulerSampleList : public std::vector<HotWeatherSchedulerSample> {
	public:
		HotWeatherSchedulerSampleList() {

			emplace_back(
				std::make_shared<HotWeatherSchedulerImpl>(nullptr, std::chrono::seconds(60), 35.0f),
				HotWeatherSchedulerDto(60, 35.0f)
			);

			emplace_back(
				std::make_shared<HotWeatherSchedulerImpl>(nullptr, std::chrono::seconds(120), 30.0f),
				HotWeatherSchedulerDto(120, 30.0f)
			);

			emplace_back(
				std::make_shared<HotWeatherSchedulerImpl>(nullptr, std::chrono::seconds(180), 33.0f),
				HotWeatherSchedulerDto(180, 33.0f)
			);

			emplace_back(
				std::make_shared<HotWeatherSchedulerImpl>(nullptr, std::chrono::seconds(150), 25.0f),
				HotWeatherSchedulerDto(150, 25.0f)
			);
		}
	};
};
