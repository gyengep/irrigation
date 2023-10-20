#pragma once
#include <memory>
#include <vector>
#include "DTO/EveryDaySchedulerDto.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	class EveryDaySchedulerSample : public Sample<EveryDaySchedulerDto> {
	public:
		EveryDaySchedulerSample() :
			Sample<EveryDaySchedulerDto>(
					EveryDaySchedulerDto(),
					"<scheduler type=\"every-day\" />"
			)
		{
		}
	};
};
