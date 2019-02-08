#pragma once
#include <memory>
#include "Schedulers/PeriodicScheduler.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<PeriodicScheduler, PeriodicSchedulerDTO> PeriodicSchedulerSample;


	class PeriodicSchedulerSample1 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample1() : PeriodicSchedulerSample(
				PeriodicScheduler(10, std::vector<bool>(), 2018, 1, 10),
				PeriodicSchedulerDTO(10, std::list<bool>(), 2018, 1, 10)
			)
		{}
	};

	class PeriodicSchedulerSample2 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample2() : PeriodicSchedulerSample(
				PeriodicScheduler(20, std::vector<bool>({ true }), 2017, 2, 11),
				PeriodicSchedulerDTO(20, std::list<bool>({ true }), 2017, 2, 11)
			)
		{}
	};

	class PeriodicSchedulerSample3 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample3() : PeriodicSchedulerSample(
				PeriodicScheduler(30, std::vector<bool>({ false, true, false, true }), 2019, 3, 12),
				PeriodicSchedulerDTO(30, std::list<bool>({ false, true, false, true }), 2019, 3, 12)
			)
		{}
	};

	class PeriodicSchedulerSample4 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample4() : PeriodicSchedulerSample(
				PeriodicScheduler(40, std::vector<bool>({ true, true, false, false, true, false, true }), 2000, 4, 13),
				PeriodicSchedulerDTO(40, std::list<bool>({ true, true, false, false, true, false, true }), 2000, 4, 13)
			)
		{}
	};
};
