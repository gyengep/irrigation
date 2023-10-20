#pragma once
#include <string>
#include <vector>
#include "DTO/WeeklySchedulerDto.h"
#include "SampleBase.h"


namespace DtoReaderWriterTestSamples {

	typedef Sample<WeeklySchedulerDto> WeeklySchedulerSample;

	class WeeklySchedulerSampleList : public std::vector<WeeklySchedulerSample> {
	public:
		WeeklySchedulerSampleList() {

			emplace_back(
				WeeklySchedulerDto(),
				"<scheduler type=\"weekly\"/>"
			);

			emplace_back(
				WeeklySchedulerDto().setValues(std::list<bool> { true, true, true, false, true, false, false }),
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
				WeeklySchedulerDto(std::list<bool> { true, false, false, true, true, false, true }),
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
