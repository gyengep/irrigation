#pragma once
#include <list>
#include <memory>
#include <vector>
#include "RunTimeSamples.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	typedef SampleList<RunTimeDTO> RunTimeContainerSample;

	class RunTimeContainerSampleList : public std::vector<RunTimeContainerSample> {
	public:
		RunTimeContainerSampleList() {

			emplace_back(
				std::list<RunTimeDTO>({
					RunTimeSampleList().at(0).getDto().setId(0),
					RunTimeSampleList().at(1).getDto().setId(1),
					RunTimeSampleList().at(2).getDto().setId(2),
					RunTimeSampleList().at(3).getDto().setId(3),
					RunTimeSampleList().at(4).getDto().setId(4),
					RunTimeSampleList().at(5).getDto().setId(5)
				}),
				"<runtimes>" +
					RunTimeSampleList().at(0).getXmlWithId(0) +
					RunTimeSampleList().at(1).getXmlWithId(1) +
					RunTimeSampleList().at(2).getXmlWithId(2) +
					RunTimeSampleList().at(3).getXmlWithId(3) +
					RunTimeSampleList().at(4).getXmlWithId(4) +
					RunTimeSampleList().at(5).getXmlWithId(5) +
				"</runtimes>"
			);

			emplace_back(
				std::list<RunTimeDTO>({
					RunTimeSampleList().at(5).getDto().setId(0),
					RunTimeSampleList().at(4).getDto().setId(1),
					RunTimeSampleList().at(3).getDto().setId(2),
					RunTimeSampleList().at(2).getDto().setId(3),
					RunTimeSampleList().at(1).getDto().setId(4),
					RunTimeSampleList().at(0).getDto().setId(5)
				}),
				"<runtimes>" +
					RunTimeSampleList().at(5).getXmlWithId(0) +
					RunTimeSampleList().at(4).getXmlWithId(1) +
					RunTimeSampleList().at(3).getXmlWithId(2) +
					RunTimeSampleList().at(2).getXmlWithId(3) +
					RunTimeSampleList().at(1).getXmlWithId(4) +
					RunTimeSampleList().at(0).getXmlWithId(5) +
				"</runtimes>"
			);

			emplace_back(
				std::list<RunTimeDTO>({
					RunTimeSampleList().at(0).getDto().setId(0),
					RunTimeSampleList().at(2).getDto().setId(1),
					RunTimeSampleList().at(4).getDto().setId(2),
					RunTimeSampleList().at(1).getDto().setId(3),
					RunTimeSampleList().at(3).getDto().setId(4),
					RunTimeSampleList().at(5).getDto().setId(5)
				}),
				"<runtimes>" +
					RunTimeSampleList().at(0).getXmlWithId(0) +
					RunTimeSampleList().at(2).getXmlWithId(1) +
					RunTimeSampleList().at(4).getXmlWithId(2) +
					RunTimeSampleList().at(1).getXmlWithId(3) +
					RunTimeSampleList().at(3).getXmlWithId(4) +
					RunTimeSampleList().at(5).getXmlWithId(5) +
				"</runtimes>"
			);

			emplace_back(
				std::list<RunTimeDTO>({
					RunTimeSampleList().at(2).getDto().setId(0),
					RunTimeSampleList().at(0).getDto().setId(1),
					RunTimeSampleList().at(5).getDto().setId(2),
					RunTimeSampleList().at(1).getDto().setId(3),
					RunTimeSampleList().at(3).getDto().setId(4),
					RunTimeSampleList().at(4).getDto().setId(5)
				}),
				"<runtimes>" +
					RunTimeSampleList().at(2).getXmlWithId(0) +
					RunTimeSampleList().at(0).getXmlWithId(1) +
					RunTimeSampleList().at(5).getXmlWithId(2) +
					RunTimeSampleList().at(1).getXmlWithId(3) +
					RunTimeSampleList().at(3).getXmlWithId(4) +
					RunTimeSampleList().at(4).getXmlWithId(5) +
				"</runtimes>"
			);
		}
	};
};
