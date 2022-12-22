#pragma once
#include <string>
#include <vector>
#include "DTO/CurrentTemperatureDTO.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	typedef Sample<CurrentTemperatureDTO> CurrentTemperatureSample;

	class CurrentTemperatureSampleList : public std::vector<CurrentTemperatureSample> {
	public:
		CurrentTemperatureSampleList() {

			emplace_back(
				CurrentTemperatureDTO("2021-10-11", "celsius", 28.0),
				"<temperature>"
					"<value>28</value>"
					"<datetime>2021-10-11</datetime>"
					"<unit>celsius</unit>"
				"</temperature>"
			);

			emplace_back(
				CurrentTemperatureDTO("2021-01-02 20:21:22", "C", 29.5),
				"<temperature>"
					"<value>29</value>"
					"<datetime>2021-01-02 20:21:22</datetime>"
					"<unit>C</unit>"
				"</temperature>"
			);

			emplace_back(
				CurrentTemperatureDTO("2021 oct 11", "unit", 30.456),
				"<temperature>"
					"<value>30</value>"
					"<datetime>2021 oct 11</datetime>"
					"<unit>unit</unit>"
				"</temperature>"
			);
		}
	};
};
