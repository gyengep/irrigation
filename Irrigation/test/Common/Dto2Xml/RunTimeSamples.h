#pragma once
#include <string>
#include <vector>
#include "DTO/RunTimeDTO.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	typedef Sample<RunTimeDTO> RunTimeSample;

	class RunTimeSampleList : public std::vector<RunTimeSample> {
	public:
		RunTimeSampleList() {

			emplace_back(
				RunTimeDTO(),
				"<runtime/>"
			);

			emplace_back(
				RunTimeDTO().setId(9348),
				"<runtime id=\"9348\"/>"
			);

			emplace_back(
				RunTimeDTO().setMinutes(17),
				"<runtime><minute>17</minute></runtime>"
			);

			emplace_back(
				RunTimeDTO().setSeconds(34),
				"<runtime><second>34</second></runtime>"
			);

			emplace_back(
				RunTimeDTO(52, 43).setId(547),
				"<runtime id=\"547\"><minute>52</minute><second>43</second></runtime>"
			);
		}
	};
};
