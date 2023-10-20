#pragma once
#include <list>
#include <string>
#include <vector>
#include "SampleBase.h"
#include "StartTimeSamples.h"


namespace DtoReaderWriterTestSamples {

	typedef ContainerSample<StartTimeDto> StartTimeListSample;

	class StartTimeListSampleList : public std::vector<StartTimeListSample> {
	public:
		StartTimeListSampleList() {

			emplace_back(
				std::list<StartTimeDto> {
				},
				"<starttimes/>"
			);

			for (const auto& startTimeSample : StartTimeSampleList()) {
				emplace_back(
					std::list<StartTimeDto> {
						startTimeSample.getDto()
					},
					"<starttimes>" +
						startTimeSample.getXml() +
					"</starttimes>"
				);
			}

			{
				std::list<StartTimeDto> startTimeDtoList;
				std::string xml;

				for (const auto& startTimeSample : StartTimeSampleList()) {
					startTimeDtoList.push_back(startTimeSample.getDto());
					xml.append(startTimeSample.getXml());
				}

				emplace_back(
					startTimeDtoList,
					"<starttimes>" +
						xml +
					"</starttimes>"
				);
			}
		}
	};
};
