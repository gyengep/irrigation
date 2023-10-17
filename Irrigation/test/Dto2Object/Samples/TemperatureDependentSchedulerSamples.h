#pragma once
#include <memory>
#include <vector>
#include "DTO/TemperatureDependentSchedulerDTO.h"
#include "Schedulers/Impl/TemperatureDependentSchedulerImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTestSamples {

	typedef ObjectSample<TemperatureDependentSchedulerImpl, TemperatureDependentSchedulerDTO> TemperatureDependentSchedulerSample;

	class TemperatureDependentSchedulerSampleList : public std::vector<TemperatureDependentSchedulerSample> {
	public:
		TemperatureDependentSchedulerSampleList() {

			emplace_back(
				std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 1.0f, 10, 20),
				TemperatureDependentSchedulerDTO(1.0f, 10, 20)
			);

			emplace_back(
				std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 2.0f, 15, 25),
				TemperatureDependentSchedulerDTO(2.0f, 15, 25)
			);

			emplace_back(
				std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 12.0f, 25, 26),
				TemperatureDependentSchedulerDTO(12.0f, 25, 26)
			);

			emplace_back(
				std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 13.0f, 25, 26),
				TemperatureDependentSchedulerDTO(13.0f, 25, 26)
			);
		}
	};
};
