#pragma once
#include <memory>
#include <vector>
#include "DTO/TemperatureDependentSchedulerDto.h"
#include "Schedulers/Impl/TemperatureDependentSchedulerImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTestSamples {

	typedef ObjectSample<TemperatureDependentSchedulerImpl, TemperatureDependentSchedulerDto> TemperatureDependentSchedulerSample;

	class TemperatureDependentSchedulerSampleList : public std::vector<TemperatureDependentSchedulerSample> {
	public:
		TemperatureDependentSchedulerSampleList() {

			emplace_back(
				std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 1.0f, 10, 20),
				TemperatureDependentSchedulerDto(1.0f, 10, 20)
			);

			emplace_back(
				std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 2.0f, 15, 25),
				TemperatureDependentSchedulerDto(2.0f, 15, 25)
			);

			emplace_back(
				std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 12.0f, 25, 26),
				TemperatureDependentSchedulerDto(12.0f, 25, 26)
			);

			emplace_back(
				std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 13.0f, 25, 26),
				TemperatureDependentSchedulerDto(13.0f, 25, 26)
			);
		}
	};
};
