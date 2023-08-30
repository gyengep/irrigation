#pragma once
#include <list>
#include <memory>
#include <vector>
#include "Logic/RunTimeContainerImpl.h"
#include "RunTimeSamples.h"
#include "SampleBase.h"


namespace Dto2ObjectTestSamples {

	typedef ContainerSample<RunTimeContainerImpl, RunTimeDTO> RunTimeContainerSample;

	class RunTimeContainerSampleList : public std::vector<RunTimeContainerSample> {
	public:
		RunTimeContainerSampleList() {

			emplace_back(
				std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
					RunTimeSampleList().at(0).getObjectPtr(),
					RunTimeSampleList().at(1).getObjectPtr(),
					RunTimeSampleList().at(2).getObjectPtr(),
					RunTimeSampleList().at(3).getObjectPtr(),
					RunTimeSampleList().at(4).getObjectPtr(),
					RunTimeSampleList().at(5).getObjectPtr()
				}),
				std::list<RunTimeDTO>({
					RunTimeSampleList().at(0).getDto().setId(0),
					RunTimeSampleList().at(1).getDto().setId(1),
					RunTimeSampleList().at(2).getDto().setId(2),
					RunTimeSampleList().at(3).getDto().setId(3),
					RunTimeSampleList().at(4).getDto().setId(4),
					RunTimeSampleList().at(5).getDto().setId(5)
				})
			);

			emplace_back(
				std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
					RunTimeSampleList().at(5).getObjectPtr(),
					RunTimeSampleList().at(4).getObjectPtr(),
					RunTimeSampleList().at(3).getObjectPtr(),
					RunTimeSampleList().at(2).getObjectPtr(),
					RunTimeSampleList().at(1).getObjectPtr(),
					RunTimeSampleList().at(0).getObjectPtr()
				}),
				std::list<RunTimeDTO>({
					RunTimeSampleList().at(5).getDto().setId(0),
					RunTimeSampleList().at(4).getDto().setId(1),
					RunTimeSampleList().at(3).getDto().setId(2),
					RunTimeSampleList().at(2).getDto().setId(3),
					RunTimeSampleList().at(1).getDto().setId(4),
					RunTimeSampleList().at(0).getDto().setId(5)
				})
			);

			emplace_back(
				std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
					RunTimeSampleList().at(0).getObjectPtr(),
					RunTimeSampleList().at(2).getObjectPtr(),
					RunTimeSampleList().at(4).getObjectPtr(),
					RunTimeSampleList().at(1).getObjectPtr(),
					RunTimeSampleList().at(3).getObjectPtr(),
					RunTimeSampleList().at(5).getObjectPtr()
				}),
				std::list<RunTimeDTO>({
					RunTimeSampleList().at(0).getDto().setId(0),
					RunTimeSampleList().at(2).getDto().setId(1),
					RunTimeSampleList().at(4).getDto().setId(2),
					RunTimeSampleList().at(1).getDto().setId(3),
					RunTimeSampleList().at(3).getDto().setId(4),
					RunTimeSampleList().at(5).getDto().setId(5)
				})
			);

			emplace_back(
				std::make_shared<RunTimeContainerImpl>(std::initializer_list<RunTimePtr> {
					RunTimeSampleList().at(2).getObjectPtr(),
					RunTimeSampleList().at(0).getObjectPtr(),
					RunTimeSampleList().at(5).getObjectPtr(),
					RunTimeSampleList().at(1).getObjectPtr(),
					RunTimeSampleList().at(3).getObjectPtr(),
					RunTimeSampleList().at(4).getObjectPtr()
				}),
				std::list<RunTimeDTO>({
					RunTimeSampleList().at(2).getDto().setId(0),
					RunTimeSampleList().at(0).getDto().setId(1),
					RunTimeSampleList().at(5).getDto().setId(2),
					RunTimeSampleList().at(1).getDto().setId(3),
					RunTimeSampleList().at(3).getDto().setId(4),
					RunTimeSampleList().at(4).getDto().setId(5)
				})
			);
		}
	};
};