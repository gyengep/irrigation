#pragma once
#include <memory>
#include <vector>
#include "DTO/EveryDaySchedulerDTO.h"
#include "Schedulers/EveryDaySchedulerImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTestSamples {

	class EveryDaySchedulerSample : public ObjectSample<EveryDaySchedulerImpl, EveryDaySchedulerDTO> {
	public:
		EveryDaySchedulerSample() :
			ObjectSample<EveryDaySchedulerImpl, EveryDaySchedulerDTO>(
					std::make_shared<EveryDaySchedulerImpl>(),
					EveryDaySchedulerDTO()
			)
		{
		}
	};
};
