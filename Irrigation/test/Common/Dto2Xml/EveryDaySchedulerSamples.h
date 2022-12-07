#pragma once
#include <string>
#include <vector>
#include "DTO/EveryDaySchedulerDTO.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	class EveryDaySchedulerSample : public Sample<EveryDaySchedulerDTO> {
	public:
		EveryDaySchedulerSample() :
			Sample<EveryDaySchedulerDTO>(
				EveryDaySchedulerDTO(),
				"<scheduler type=\"every-day\"/>"
			)
		{
		}
	};
};
