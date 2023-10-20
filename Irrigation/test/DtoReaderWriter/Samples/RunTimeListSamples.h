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
				std::list<RunTimeDto> {
				},
				"<runtimes/>"
			);

			for (const auto& runTimeSample : RunTimeSampleList()) {
				emplace_back(
					std::list<RunTimeDto> {
						runTimeSample.getDto()
					},
					"<runtimes>" +
						runTimeSample.getXml() +
					"</runtimes>"
				);
			}

			{
				std::list<RunTimeDto> runTimeDtoList;
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
