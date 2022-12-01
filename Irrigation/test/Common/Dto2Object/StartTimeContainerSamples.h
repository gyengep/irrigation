#pragma once
#include <list>
#include <memory>
#include <vector>
#include "Logic/StartTimeContainerImpl.h"
#include "StartTimeSamples.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ContainerSample<StartTimeContainerImpl, StartTimeDTO> StartTimeContainerSample;

	class StartTimeContainerSampleList : public std::vector<StartTimeContainerSample> {
	public:
		StartTimeContainerSampleList() {

			emplace_back(
				std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
				}),
				std::list<StartTimeDTO>({
				})
			);

			emplace_back(
				std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
					{ 10, StartTimeSampleList().at(0).getObjectPtr() }
				}),
				std::list<StartTimeDTO>({
					StartTimeSampleList().at(0).getDto().setId(10)
				})
			);

			emplace_back(
				std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
					{ 100, StartTimeSampleList().at(0).getObjectPtr() },
					{ 101, StartTimeSampleList().at(1).getObjectPtr() },
					{ 102, StartTimeSampleList().at(2).getObjectPtr() }
				}),
				std::list<StartTimeDTO>({
					StartTimeSampleList().at(0).getDto().setId(100),
					StartTimeSampleList().at(1).getDto().setId(101),
					StartTimeSampleList().at(2).getDto().setId(102)
				})
			);

			emplace_back(
				std::make_shared<StartTimeContainerImpl>(std::initializer_list<StartTimeContainer::value_type> {
					{ 200, StartTimeSampleList().at(3).getObjectPtr() },
					{ 300, StartTimeSampleList().at(2).getObjectPtr() },
					{ 400, StartTimeSampleList().at(0).getObjectPtr() },
					{ 500, StartTimeSampleList().at(1).getObjectPtr() }
				}),
				std::list<StartTimeDTO>({
					StartTimeSampleList().at(3).getDto().setId(200),
					StartTimeSampleList().at(2).getDto().setId(300),
					StartTimeSampleList().at(0).getDto().setId(400),
					StartTimeSampleList().at(1).getDto().setId(500)
				})
			);
		}
	};
};
