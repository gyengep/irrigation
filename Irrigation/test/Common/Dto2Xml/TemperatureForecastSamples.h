#pragma once
#include <string>
#include <vector>
#include "DTO/TemperatureForecastDTO.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	typedef Sample<TemperatureForecastDTO> TemperatureForecastSample;

	class TemperatureForecastSampleList : public std::vector<TemperatureForecastSample> {
	public:
		TemperatureForecastSampleList() {

			emplace_back(
				TemperatureForecastDTO("2021-10-11", "2021-10-12", "celsius", 28.0, 30.0),
				"<temperature>"
					"<unit>celsius</unit>"
					"<forecast>"
						"<value-min>28</value-min>"
						"<value-max>30</value-max>"
						"<datetime-from>2021-10-11</datetime-from>"
						"<datetime-to>2021-10-12</datetime-to>"
					"</forecast>"
				"</temperature>"
			);

			emplace_back(
				TemperatureForecastDTO("2021-01-02 20:21:22", "2021-01-02 10:11:12", "C", 25.5, 29.6),
				"<temperature>"
					"<unit>C</unit>"
					"<forecast>"
						"<value-min>25</value-min>"
						"<value-max>29</value-max>"
						"<datetime-from>2021-01-02 20:21:22</datetime-from>"
						"<datetime-to>2021-01-02 10:11:12</datetime-to>"
					"</forecast>"
				"</temperature>"
			);

			emplace_back(
				TemperatureForecastDTO("2021 oct 11", "2022 jan 03", "unit", 30.456, 31.234),
				"<temperature>"
					"<unit>unit</unit>"
					"<forecast>"
						"<value-min>30</value-min>"
						"<value-max>31</value-max>"
						"<datetime-from>2021 oct 11</datetime-from>"
						"<datetime-to>2022 jan 03</datetime-to>"
					"</forecast>"
				"</temperature>"
			);
		}
	};
};
