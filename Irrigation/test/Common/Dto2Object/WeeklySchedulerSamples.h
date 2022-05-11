#pragma once
#include <memory>
#include "Schedulers/WeeklyScheduler.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<WeeklyScheduler, WeeklySchedulerDTO> WeeklySchedulerSample;


	class WeeklySchedulerSample1 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample1() : WeeklySchedulerSample(
			std::make_shared<WeeklyScheduler>(std::array<bool, 7>({ true, false, true, false, true, false, true })),
			WeeklySchedulerDTO(std::list<bool>({ true, false, true, false, true, false, true }))
		) {}
	};

	class WeeklySchedulerSample2 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample2() : WeeklySchedulerSample(
			std::make_shared<WeeklyScheduler>(std::array<bool, 7>({ false, true, false, true, false, true, false })),
			WeeklySchedulerDTO(std::list<bool>({ false, true, false, true, false, true, false }))
		) {}
	};

	class WeeklySchedulerSample3 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample3() : WeeklySchedulerSample(
			std::make_shared<WeeklyScheduler>(std::array<bool, 7>({ true, true, false, false, true, false, true })),
			WeeklySchedulerDTO(std::list<bool>({ true, true, false, false, true, false, true }))
		) {}
	};

	class WeeklySchedulerSample4 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample4() : WeeklySchedulerSample(
			std::make_shared<WeeklyScheduler>(std::array<bool, 7>({ false, false, false, false, true, false, true })),
			WeeklySchedulerDTO(std::list<bool>({ false, false, false, false, true, false, true }))
		) {}
	};
};
