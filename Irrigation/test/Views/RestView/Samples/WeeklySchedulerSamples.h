#pragma once
#include <memory>
#include <vector>
#include "DTO/WeeklySchedulerDTO.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	typedef Sample<WeeklySchedulerDTO> WeeklySchedulerSample;

	class WeeklySchedulerSampleList : public std::vector<WeeklySchedulerSample> {
	public:
		WeeklySchedulerSampleList() {

			emplace_back(
				WeeklySchedulerDTO(std::list<bool> { true, false, true, false, true, false, true }),
				"<scheduler type=\"weekly\">"
					"<days>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
					"</days>"
				"</scheduler>"
			);

			emplace_back(
				WeeklySchedulerDTO(std::list<bool> { false, true, false, true, false, true, false }),
				"<scheduler type=\"weekly\">"
					"<days>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
					"</days>"
				"</scheduler>"
			);

			emplace_back(
				WeeklySchedulerDTO(std::list<bool> { true, true, false, false, true, false, true }),
				"<scheduler type=\"weekly\">"
					"<days>"
						"<day>true</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
					"</days>"
				"</scheduler>"
			);

			emplace_back(
				WeeklySchedulerDTO(std::list<bool> { false, false, false, false, true, false, true }),
				"<scheduler type=\"weekly\">"
					"<days>"
						"<day>false</day>"
						"<day>false</day>"
						"<day>false</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>true</day>"
						"<day>true</day>"
					"</days>"
				"</scheduler>"
			);
		}
	};
};
