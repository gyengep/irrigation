#pragma once
#include <string>
#include <utility>
#include <vector>
#include "DTO/TemperatureForecastDTO.h"
#include "DTO/TemperatureHistoryDTO.h"
#include "SampleBase.h"


namespace DtoReaderWriterTestSamples {

	typedef Sample<std::pair<TemperatureHistoryDTO, TemperatureForecastDTO>> TemperatureHistoryAndForecastSample;

	class TemperatureHistoryAndForecastSampleList : public std::vector<TemperatureHistoryAndForecastSample> {
	public:
		TemperatureHistoryAndForecastSampleList() {

			emplace_back(std::make_pair(
				TemperatureHistoryDTO("2021-10-11", "2021-10-12", "celsius", 28.0, 30.0, 29.0),
				TemperatureForecastDTO("2021-10-11", "2021-10-12", "celsius", 28.0, 30.0)),
				"<temperature>"
					"<unit>celsius</unit>"
					"<historical>"
						"<value-min>28</value-min>"
						"<value-max>30</value-max>"
						"<value-avg>29</value-avg>"
						"<datetime-from>2021-10-11</datetime-from>"
						"<datetime-to>2021-10-12</datetime-to>"
					"</historical>"
					"<forecast>"
						"<value-min>28</value-min>"
						"<value-max>30</value-max>"
						"<datetime-from>2021-10-11</datetime-from>"
						"<datetime-to>2021-10-12</datetime-to>"
					"</forecast>"
				"</temperature>"
			);

			emplace_back(std::make_pair(
				TemperatureHistoryDTO("2021-01-02 20:21:22", "2021-01-02 10:11:12", "C", 29.5, 25.6, 30.1),
				TemperatureForecastDTO("2021-01-02 20:21:22", "2021-01-02 10:11:12", "C", 25.5, 29.6)),
				"<temperature>"
					"<unit>C</unit>"
					"<historical>"
						"<value-min>29</value-min>"
						"<value-max>25</value-max>"
						"<value-avg>30</value-avg>"
						"<datetime-from>2021-01-02 20:21:22</datetime-from>"
						"<datetime-to>2021-01-02 10:11:12</datetime-to>"
					"</historical>"
					"<forecast>"
						"<value-min>25</value-min>"
						"<value-max>29</value-max>"
						"<datetime-from>2021-01-02 20:21:22</datetime-from>"
						"<datetime-to>2021-01-02 10:11:12</datetime-to>"
					"</forecast>"
				"</temperature>"
			);

			emplace_back(std::make_pair(
				TemperatureHistoryDTO("2021 oct 11", "2022 jan 03", "unit", 30.456, 31.234, 33.235),
				TemperatureForecastDTO("2021 oct 11", "2022 jan 03", "unit", 30.456, 31.234)),
				"<temperature>"
					"<unit>unit</unit>"
					"<historical>"
						"<value-min>30</value-min>"
						"<value-max>31</value-max>"
						"<value-avg>33</value-avg>"
						"<datetime-from>2021 oct 11</datetime-from>"
						"<datetime-to>2022 jan 03</datetime-to>"
					"</historical>"
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
