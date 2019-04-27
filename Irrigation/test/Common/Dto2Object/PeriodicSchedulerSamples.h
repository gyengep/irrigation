#pragma once
#include <memory>
#include "Schedulers/PeriodicScheduler.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<PeriodicScheduler, PeriodicSchedulerDTO> PeriodicSchedulerSample;


	class PeriodicSchedulerSample1 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample1() : PeriodicSchedulerSample(
				PeriodicScheduler(std::vector<bool>(), 2018, 1, 10),
				PeriodicSchedulerDTO(std::list<bool>(), 2018, 1, 10)
			)
		{}
	};

	class PeriodicSchedulerSample2 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample2() : PeriodicSchedulerSample(
				PeriodicScheduler(std::vector<bool>({ true }), 2017, 2, 11),
				PeriodicSchedulerDTO(std::list<bool>({ true }), 2017, 2, 11)
			)
		{}
	};

	class PeriodicSchedulerSample3 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample3() : PeriodicSchedulerSample(
				PeriodicScheduler(std::vector<bool>({ false, true, false, true }), 2019, 3, 12),
				PeriodicSchedulerDTO(std::list<bool>({ false, true, false, true }), 2019, 3, 12)
			)
		{}
	};

	class PeriodicSchedulerSample4 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample4() : PeriodicSchedulerSample(
				PeriodicScheduler(std::vector<bool>({ true, true, false, false, true, false, true }), 2000, 4, 13),
				PeriodicSchedulerDTO(std::list<bool>({ true, true, false, false, true, false, true }), 2000, 4, 13)
			)
		{}
	};
};
