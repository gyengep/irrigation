#pragma once
#include <list>
#include <string>
#include <vector>
#include "SampleBase.h"
#include "RunTimeSamples.h"


namespace DtoReaderWriterTestSamples {

	typedef ContainerSample<RunTimeDto> RunTimeListSample;

	class RunTimeListSampleList : public std::vector<RunTimeListSample> {
	public:
		RunTimeListSampleList() {

			emplace_back(
				RunTimeDtoList {
				},
				"<runtimes/>"
			);

			for (const auto& runTimeSample : RunTimeSampleList()) {
				emplace_back(
					RunTimeDtoList {
						runTimeSample.getDto()
					},
					"<runtimes>" +
						runTimeSample.getXml() +
					"</runtimes>"
				);
			}

			{
				RunTimeDtoList runTimeDtoList;
				std::string xml;

				for (const auto& runTimeSample : RunTimeSampleList()) {
					runTimeDtoList.push_back(runTimeSample.getDto());
					xml.append(runTimeSample.getXml());
				}

				emplace_back(
					runTimeDtoList,
					"<runtimes>" +
						xml +
					"</runtimes>"
				);
			}
		}
	};
};
