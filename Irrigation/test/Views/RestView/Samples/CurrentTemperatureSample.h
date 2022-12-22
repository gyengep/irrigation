#pragma once
#include "DTO/CurrentTemperatureDTO.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	class CurrentTemperatureSample : public Sample<CurrentTemperatureDTO> {
	public:
		CurrentTemperatureSample() : Sample<CurrentTemperatureDTO>(
				CurrentTemperatureDTO("Tue, 22 Jun 2021 23:36:57 +0200", "celsius", 28.1),
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

