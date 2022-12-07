#pragma once
#include <list>
#include <string>
#include <vector>
#include "SampleBase.h"
#include "StartTimeSamples.h"


namespace Dto2XmlTest {

	typedef ContainerSample<StartTimeDTO> StartTimeListSample;

	class StartTimeListSampleList : public std::vector<StartTimeListSample> {
	public:
		StartTimeListSampleList() {

			emplace_back(
				std::list<StartTimeDTO> {
				},
				"<starttimes/>"
			);

			for (const auto& startTimeSample : StartTimeSampleList()) {
				emplace_back(
					std::list<StartTimeDTO> {
						startTimeSample.getDto()
					},
					"<starttimes>" +
						startTimeSample.getXml() +
					"</starttimes>"
				);
			}

			{
				std::list<StartTimeDTO> startTimeDtoList;
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
