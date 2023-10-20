#pragma once
#include "DTO/CurrentTemperatureDto.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	class CurrentTemperatureSample : public Sample<CurrentTemperatureDto> {
	public:
		CurrentTemperatureSample() : Sample<CurrentTemperatureDto>(
				CurrentTemperatureDto("Tue, 22 Jun 2021 23:36:57 +0200", "celsius", 28.1),
				"<temperature>"
					"<value>28</value>"
					"<datetime>Tue, 22 Jun 2021 23:36:57 +0200</datetime>"
					"<unit>celsius</unit>"
				"</temperature>"
			)
		{
		}
	};

}

