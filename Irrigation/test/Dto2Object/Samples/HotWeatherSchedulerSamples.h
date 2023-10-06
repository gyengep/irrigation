#pragma once
#include <memory>
#include <vector>
#include "DTO/HotWeatherSchedulerDTO.h"
#include "Schedulers/HotWeatherSchedulerImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTestSamples {

	typedef ObjectSample<HotWeatherSchedulerImpl, HotWeatherSchedulerDTO> HotWeatherSchedulerSample;

	class HotWeatherSchedulerSampleList : public std::vector<HotWeatherSchedulerSample> {
	public:
		HotWeatherSchedulerSampleList() {

			emplace_back(
				std::make_shared<HotWeatherSchedulerImpl>(nullptr, nullptr, std::chrono::seconds(60), 35.0f),
				HotWeatherSchedulerDTO(60, 35.0f)
			);

			emplace_back(
				std::make_shared<HotWeatherSchedulerImpl>(nullptr, nullptr, std::chrono::seconds(120), 30.0f),
				HotWeatherSchedulerDTO(120, 30.0f)
			);

			emplace_back(
				std::make_shared<HotWeatherSchedulerImpl>(nullptr, nullptr, std::chrono::seconds(180), 33.0f),
				HotWeatherSchedulerDTO(180, 33.0f)
			);

			emplace_back(
				std::make_shared<HotWeatherSchedulerImpl>(nullptr, nullptr, std::chrono::seconds(150), 25.0f),
				HotWeatherSchedulerDTO(150, 25.0f)
			);
		}
	};
};
