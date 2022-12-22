#pragma once
#include <string>
#include <vector>
#include "DTO/WeeklySchedulerDTO.h"
#include "SampleBase.h"


namespace DtoReaderWriterTestSamples {

	typedef Sample<WeeklySchedulerDTO> WeeklySchedulerSample;

	class WeeklySchedulerSampleList : public std::vector<WeeklySchedulerSample> {
	public:
		WeeklySchedulerSampleList() {

			emplace_back(
				WeeklySchedulerDTO(),
				"<scheduler type=\"weekly\"/>"
			);

			emplace_back(
				WeeklySchedulerDTO().setValues(std::list<bool> { true, true, true, false, true, false, false }),
				"<scheduler type=\"weekly\">"
					"<days>"
						"<day>true</day>"
						"<day>true</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>false</day>"
					"</days>"
				"</scheduler>"
			);

			emplace_back(
				WeeklySchedulerDTO(std::list<bool> { true, false, false, true, true, false, true }),
				"<scheduler type=\"weekly\">"
					"<days>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
					"</days>"
				"</scheduler>"
			);
		}
	};
};
