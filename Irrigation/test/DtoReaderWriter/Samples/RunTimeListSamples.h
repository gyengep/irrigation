#pragma once
#include <list>
#include <string>
#include <vector>
#include "SampleBase.h"
#include "RunTimeSamples.h"


namespace DtoReaderWriterTestSamples {

	typedef ContainerSample<RunTimeDTO> RunTimeListSample;

	class RunTimeListSampleList : public std::vector<RunTimeListSample> {
	public:
		RunTimeListSampleList() {

			emplace_back(
				std::list<RunTimeDTO> {
				},
				"<runtimes/>"
			);

			for (const auto& runTimeSample : RunTimeSampleList()) {
				emplace_back(
					std::list<RunTimeDTO> {
						runTimeSample.getDto()
					},
					"<runtimes>" +
						runTimeSample.getXml() +
					"</runtimes>"
				);
			}

			{
				std::list<RunTimeDTO> runTimeDtoList;
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
