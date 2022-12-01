#pragma once
#include <memory>
#include <vector>
#include "DTO/WeeklySchedulerDTO.h"
#include "Schedulers/WeeklySchedulerImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<WeeklySchedulerImpl, WeeklySchedulerDTO> WeeklySchedulerSample;

	class WeeklySchedulerSampleList : public std::vector<WeeklySchedulerSample> {
	public:
		WeeklySchedulerSampleList() {

			emplace_back(
				std::make_shared<WeeklySchedulerImpl>(std::initializer_list<bool>({ true, false, true, false, true, false, true })),
				WeeklySchedulerDTO(std::list<bool>({ true, false, true, false, true, false, true }))
			);

			emplace_back(
				std::make_shared<WeeklySchedulerImpl>(std::initializer_list<bool>({ false, true, false, true, false, true, false })),
				WeeklySchedulerDTO(std::list<bool>({ false, true, false, true, false, true, false }))
			);

			emplace_back(
				std::make_shared<WeeklySchedulerImpl>(std::initializer_list<bool>({ true, true, false, false, true, false, true })),
				WeeklySchedulerDTO(std::list<bool>({ true, true, false, false, true, false, true }))
			);

			emplace_back(
				std::make_shared<WeeklySchedulerImpl>(std::initializer_list<bool>({ false, false, false, false, true, false, true })),
				WeeklySchedulerDTO(std::list<bool>({ false, false, false, false, true, false, true }))
			);
		}
	};
};
