#pragma once
#include <list>
#include <memory>
#include <vector>
#include "StartTimeSamples.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	typedef SampleList<StartTimeDTO> StartTimeContainerSample;

	class StartTimeContainerSampleList : public std::vector<StartTimeContainerSample> {
	public:
		StartTimeContainerSampleList() {

			emplace_back(
				std::list<StartTimeDTO>({
				}),
				"<starttimes />"
			);

			emplace_back(
				std::list<StartTimeDTO>({
					StartTimeSampleList().at(0).getDto().setId(10)
				}),
				"<starttimes>" +
					StartTimeSampleList().at(0).getXmlWithId(10) +
				"</starttimes>"
			);

			emplace_back(
				std::list<StartTimeDTO>({
					StartTimeSampleList().at(0).getDto().setId(100),
					StartTimeSampleList().at(1).getDto().setId(101),
					StartTimeSampleList().at(2).getDto().setId(102)
				}),
				"<starttimes>" +
					StartTimeSampleList().at(0).getXmlWithId(100) +
					StartTimeSampleList().at(1).getXmlWithId(101) +
					StartTimeSampleList().at(2).getXmlWithId(102) +
				"</starttimes>"
			);

			emplace_back(
				std::list<StartTimeDTO>({
					StartTimeSampleList().at(3).getDto().setId(200),
					StartTimeSampleList().at(2).getDto().setId(300),
					StartTimeSampleList().at(0).getDto().setId(400),
					StartTimeSampleList().at(1).getDto().setId(500)
				}),
				"<starttimes>" +
					StartTimeSampleList().at(3).getXmlWithId(200) +
					StartTimeSampleList().at(2).getXmlWithId(300) +
					StartTimeSampleList().at(0).getXmlWithId(400) +
					StartTimeSampleList().at(1).getXmlWithId(500) +
				"</starttimes>"
			);
		}
	};
};
