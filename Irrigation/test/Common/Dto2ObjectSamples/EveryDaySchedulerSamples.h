#pragma once
#include <memory>
#include <vector>
#include "DTO/EveryDaySchedulerDto.h"
#include "Schedulers/Impl/EveryDaySchedulerImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTestSamples {

	class EveryDaySchedulerSample : public ObjectSample<EveryDaySchedulerImpl, EveryDaySchedulerDto> {
	public:
		EveryDaySchedulerSample() :
			ObjectSample<EveryDaySchedulerImpl, EveryDaySchedulerDto>(
					std::make_shared<EveryDaySchedulerImpl>(),
					EveryDaySchedulerDto()
			)
		{
		}
	};
};
