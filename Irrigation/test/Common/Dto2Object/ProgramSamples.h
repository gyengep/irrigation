#pragma once
#include <memory>
#include <vector>
#include "DTO/ProgramDTO.h"
#include "Logic/ProgramImpl.h"
#include "RunTimeContainerSamples.h"
#include "StartTimeContainerSamples.h"
#include "SchedulerContainerSamples.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<ProgramImpl, ProgramDTO> ProgramSample;

	class ProgramSampleList : public std::vector<ProgramSample> {
	public:
		ProgramSampleList() {

			emplace_back(
				std::make_shared<ProgramImpl>(
					false, "program1", 54, SchedulerType::EVERY_DAY,
					SchedulerContainerSampleList().at(0).getObjectPtr(),
					RunTimeContainerSampleList().at(0).getContainerPtr(),
					StartTimeContainerSampleList().at(0).getContainerPtr()
				),
				ProgramDTO(
					false, "program1", 54, to_string(SchedulerType::EVERY_DAY),
					SchedulerContainerSampleList().at(0).getDto(),
					RunTimeContainerSampleList().at(0).getDtoList(),
					StartTimeContainerSampleList().at(0).getDtoList()
				)
			);

			emplace_back(
				std::make_shared<ProgramImpl>(
					true, "program2", 61, SchedulerType::HOT_WEATHER,
					SchedulerContainerSampleList().at(1).getObjectPtr(),
					RunTimeContainerSampleList().at(1).getContainerPtr(),
					StartTimeContainerSampleList().at(1).getContainerPtr()
				),
				ProgramDTO(
					true, "program2", 61, to_string(SchedulerType::HOT_WEATHER),
					SchedulersDTO(SchedulerContainerSampleList().at(1).getDto()),
					std::list<RunTimeDTO>(RunTimeContainerSampleList().at(1).getDtoList()),
					std::list<StartTimeDTO>(StartTimeContainerSampleList().at(1).getDtoList())
				)
			);

			emplace_back(
				std::make_shared<ProgramImpl>(
					false, "program3", 159, SchedulerType::TEMPERATURE_DEPENDENT,
					SchedulerContainerSampleList().at(2).getObjectPtr(),
					RunTimeContainerSampleList().at(2).getContainerPtr(),
					StartTimeContainerSampleList().at(2).getContainerPtr()
				),
				ProgramDTO(
					false, "program3", 159, to_string(SchedulerType::TEMPERATURE_DEPENDENT),
					SchedulersDTO(SchedulerContainerSampleList().at(2).getDto()),
					std::list<RunTimeDTO>(RunTimeContainerSampleList().at(2).getDtoList()),
					std::list<StartTimeDTO>(StartTimeContainerSampleList().at(2).getDtoList())
				)
			);

			emplace_back(
				std::make_shared<ProgramImpl>(
					true, "program4", 238, SchedulerType::WEEKLY,
					SchedulerContainerSampleList().at(3).getObjectPtr(),
					RunTimeContainerSampleList().at(3).getContainerPtr(),
					StartTimeContainerSampleList().at(3).getContainerPtr()
				),
				ProgramDTO(
					true, "program4", 238, to_string(SchedulerType::WEEKLY),
					SchedulersDTO(SchedulerContainerSampleList().at(3).getDto()),
					std::list<RunTimeDTO>(RunTimeContainerSampleList().at(3).getDtoList()),
					std::list<StartTimeDTO>(StartTimeContainerSampleList().at(3).getDtoList())
				)
			);
		}
	};
};
