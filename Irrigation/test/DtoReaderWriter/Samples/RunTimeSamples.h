#pragma once
#include <string>
#include <vector>
#include "DTO/RunTimeDto.h"
#include "SampleBase.h"


namespace DtoReaderWriterTestSamples {

	typedef Sample<RunTimeDto> RunTimeSample;

	class RunTimeSampleList : public std::vector<RunTimeSample> {
	public:
		RunTimeSampleList() {

			emplace_back(
				RunTimeDto(),
				"<runtime/>"
			);

			emplace_back(
				RunTimeDto().setId(9348),
				"<runtime id=\"9348\"/>"
			);

			emplace_back(
				RunTimeDto().setMinutes(17),
				"<runtime><minute>17</minute></runtime>"
			);

			emplace_back(
				RunTimeDto().setSeconds(34),
				"<runtime><second>34</second></runtime>"
			);

			emplace_back(
				RunTimeDto(52, 43).setId(547),
				"<runtime id=\"547\"><minute>52</minute><second>43</second></runtime>"
			);
		}
	};
};
