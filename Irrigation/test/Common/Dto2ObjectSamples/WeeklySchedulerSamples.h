#pragma once
#include <memory>
#include <vector>
#include "DTO/WeeklySchedulerDto.h"
#include "Schedulers/Impl/WeeklySchedulerImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTestSamples {

	typedef ObjectSample<WeeklySchedulerImpl, WeeklySchedulerDto> WeeklySchedulerSample;

	class WeeklySchedulerSampleList : public std::vector<WeeklySchedulerSample> {
	public:
		WeeklySchedulerSampleList() {

			emplace_back(
				std::make_shared<WeeklySchedulerImpl>(std::initializer_list<bool>({ true, false, true, false, true, false, true })),
				WeeklySchedulerDto(std::list<bool> { true, false, true, false, true, false, true })
			);

			emplace_back(
				std::make_shared<WeeklySchedulerImpl>(std::initializer_list<bool>({ false, true, false, true, false, true, false })),
				WeeklySchedulerDto(std::list<bool> { false, true, false, true, false, true, false })
			);

			emplace_back(
				std::make_shared<WeeklySchedulerImpl>(std::initializer_list<bool>({ true, true, false, false, true, false, true })),
				WeeklySchedulerDto(std::list<bool> { true, true, false, false, true, false, true })
			);

			emplace_back(
				std::make_shared<WeeklySchedulerImpl>(std::initializer_list<bool>({ false, false, false, false, true, false, true })),
				WeeklySchedulerDto(std::list<bool> { false, false, false, false, true, false, true })
			);
		}
	};
};
