#pragma once
#include <memory>
#include "Schedulers/EveryDayScheduler.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<EveryDayScheduler, EveryDaySchedulerDTO> EveryDaySchedulerSample;


	class EveryDaySchedulerSample1 : public EveryDaySchedulerSample {
	public:
		EveryDaySchedulerSample1() : EveryDaySchedulerSample(
			std::make_shared<EveryDayScheduler>(),
			EveryDaySchedulerDTO()
		) {}
	};
};
