#pragma once
#include <memory>
#include "Schedulers/WeeklyScheduler.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<WeeklyScheduler, WeeklySchedulerDTO> WeeklySchedulerSample;


	class WeeklySchedulerSample1 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample1() : WeeklySchedulerSample(
				WeeklyScheduler(10, std::array<bool, 7>({ true, false, true, false, true, false, true })),
				WeeklySchedulerDTO(10, std::list<bool>({ true, false, true, false, true, false, true }))
			)
		{}
	};

	class WeeklySchedulerSample2 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample2() : WeeklySchedulerSample(
				WeeklyScheduler(20, std::array<bool, 7>({ false, true, false, true, false, true, false })),
				WeeklySchedulerDTO(20, std::list<bool>({ false, true, false, true, false, true, false }))
			)
		{}
	};

	class WeeklySchedulerSample3 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample3() : WeeklySchedulerSample(
				WeeklyScheduler(30, std::array<bool, 7>({ true, true, false, false, true, false, true })),
				WeeklySchedulerDTO(30, std::list<bool>({ true, true, false, false, true, false, true }))
			)
		{}
	};

	class WeeklySchedulerSample4 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample4() : WeeklySchedulerSample(
				WeeklyScheduler(40, std::array<bool, 7>({ false, false, false, false, true, false, true })),
				WeeklySchedulerDTO(40, std::list<bool>({ false, false, false, false, true, false, true }))
			)
		{}
	};
};
