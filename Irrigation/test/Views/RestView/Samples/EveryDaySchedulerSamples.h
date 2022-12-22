#pragma once
#include <memory>
#include <vector>
#include "DTO/EveryDaySchedulerDTO.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	class EveryDaySchedulerSample : public Sample<EveryDaySchedulerDTO> {
	public:
		EveryDaySchedulerSample() :
			Sample<EveryDaySchedulerDTO>(
					EveryDaySchedulerDTO(),
					"<scheduler type=\"every-day\" />"
			)
		{
		}
	};
};
