#pragma once
#include <list>
#include <memory>
#include <vector>
#include "Logic/ProgramContainerImpl.h"
#include "ProgramSamples.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ContainerSample<ProgramContainerImpl, ProgramDTO> ProgramContainerSample;

	class ProgramContainerSampleList : public std::vector<ProgramContainerSample> {
	public:
		ProgramContainerSampleList() {

			emplace_back(
				std::make_shared<ProgramContainerImpl>(std::initializer_list<ProgramContainer::value_type>{
				}),
				std::list<ProgramDTO>({
				})
			);

			emplace_back(
				std::make_shared<ProgramContainerImpl>(std::initializer_list<ProgramContainer::value_type>{
					{ 10, ProgramSampleList().at(0).getObjectPtr() }
				}),
				std::list<ProgramDTO>({
					ProgramSampleList().at(0).getDto().setId(10)
				})
			);

			emplace_back(
				std::make_shared<ProgramContainerImpl>(std::initializer_list<ProgramContainer::value_type>{
					{ 100, ProgramSampleList().at(0).getObjectPtr() },
					{ 101, ProgramSampleList().at(1).getObjectPtr() },
					{ 102, ProgramSampleList().at(2).getObjectPtr() }
				}),
				std::list<ProgramDTO>({
					ProgramSampleList().at(0).getDto().setId(100),
					ProgramSampleList().at(1).getDto().setId(101),
					ProgramSampleList().at(2).getDto().setId(102)
				})
			);

			emplace_back(
				std::make_shared<ProgramContainerImpl>(std::initializer_list<ProgramContainer::value_type>{
					{ 200, ProgramSampleList().at(3).getObjectPtr() },
					{ 300, ProgramSampleList().at(2).getObjectPtr() },
					{ 400, ProgramSampleList().at(0).getObjectPtr() },
					{ 500, ProgramSampleList().at(1).getObjectPtr() }
				}),
				std::list<ProgramDTO>({
					ProgramSampleList().at(3).getDto().setId(200),
					ProgramSampleList().at(2).getDto().setId(300),
					ProgramSampleList().at(0).getDto().setId(400),
					ProgramSampleList().at(1).getDto().setId(500)
				})
			);
		}
	};
};
