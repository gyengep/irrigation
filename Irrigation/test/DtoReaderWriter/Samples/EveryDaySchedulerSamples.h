#pragma once
#include <string>
#include <vector>
#include "DTO/EveryDaySchedulerDto.h"
#include "SampleBase.h"


namespace DtoReaderWriterTestSamples {

	class EveryDaySchedulerSample : public Sample<EveryDaySchedulerDto> {
	public:
		EveryDaySchedulerSample() :
			Sample<EveryDaySchedulerDto>(
				EveryDaySchedulerDto(),
				"<scheduler type=\"every-day\"/>"
			)
		{
		}
	};
};
