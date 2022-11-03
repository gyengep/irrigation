#pragma once
#include <memory>
#include "Logic/ProgramImpl.h"
#include "Logic/ProgramImplBuilder.h"
#include "SchedulerContainerSamples.h"
#include "RunTimeListSamples.h"
#include "StartTimeListSamples.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	class ProgramSample : public ObjectSample<Program, ProgramDTO> {
	public:
		ProgramSample(const std::shared_ptr<Program>& objectPtr, ProgramDTO&& dto) : ObjectSample<Program, ProgramDTO>(objectPtr, std::move(dto)) {}
		virtual std::shared_ptr<ProgramImpl::Builder> createBuilder() const = 0;
	};


	class ProgramSample1 : public ProgramSample {
	public:
		ProgramSample1() : ProgramSample(
			createBuilder()->build(),
			ProgramDTO(
				false, "name1", 54, to_string(SchedulerType::EVERY_DAY),
				SchedulersDTO(SchedulerContainerSample1().getDto()),
				std::list<RunTimeDTO>(RunTimeListSample1().getDtoList()),
				std::list<StartTimeDTO>(StartTimeListSample1().getDtoList())
			)
		) {}

		virtual std::shared_ptr<ProgramImpl::Builder> createBuilder() const override {
			auto builder = std::make_shared<ProgramImpl::Builder>();

			builder->setEnabled(false).
				setName("name1").
				setAdjustment(54).
				setSchedulerType(SchedulerType::EVERY_DAY).
				setSchedulerContainer(SchedulerContainerSample1().getObjectPtr()).
				setRunTimeContainer(RunTimeListSample1().getContainerPtr()).
				setStartTimeContainer(StartTimeListSample1().getContainerPtr());

			return builder;
		}
	};

	class ProgramSample2 : public ProgramSample {
	public:
		ProgramSample2() : ProgramSample(
			createBuilder()->build(),
			ProgramDTO(true, "name2", 61, to_string(SchedulerType::WEEKLY),
				SchedulersDTO(SchedulerContainerSample2().getDto()),
				std::list<RunTimeDTO>(RunTimeListSample2().getDtoList()),
				std::list<StartTimeDTO>(StartTimeListSample2().getDtoList())
			)
		) {}

		virtual std::shared_ptr<ProgramImpl::Builder> createBuilder() const override {
			auto builder = std::make_shared<ProgramImpl::Builder>();

			builder->
				setEnabled(true).
				setName("name2").
				setAdjustment(61).
				setSchedulerType(SchedulerType::WEEKLY).
				setSchedulerContainer(SchedulerContainerSample2().getObjectPtr()).
				setRunTimeContainer(RunTimeListSample2().getContainerPtr()).
				setStartTimeContainer(StartTimeListSample2().getContainerPtr());

			return builder;
		}
	};

	class ProgramSample3 : public ProgramSample {
	public:
		ProgramSample3() : ProgramSample(
			createBuilder()->build(),
			ProgramDTO(
				false, "name3", 159, to_string(SchedulerType::WEEKLY),
				SchedulersDTO(SchedulerContainerSample3().getDto()),
				std::list<RunTimeDTO>(RunTimeListSample3().getDtoList()),
				std::list<StartTimeDTO>(StartTimeListSample3().getDtoList())
			)
		) {}

		virtual std::shared_ptr<ProgramImpl::Builder> createBuilder() const override {
			auto builder = std::make_shared<ProgramImpl::Builder>();

			builder->
				setEnabled(false).
				setName("name3").
				setAdjustment(159).
				setSchedulerType(SchedulerType::WEEKLY).
				setSchedulerContainer(SchedulerContainerSample3().getObjectPtr()).
				setRunTimeContainer(RunTimeListSample3().getContainerPtr()).
				setStartTimeContainer(StartTimeListSample3().getContainerPtr());

			return builder;
		}
	};

	class ProgramSample4 : public ProgramSample {
	public:
		ProgramSample4() : ProgramSample(
			createBuilder()->build(),
			ProgramDTO(
				true, "name4", 238, to_string(SchedulerType::WEEKLY),
				SchedulersDTO(SchedulerContainerSample4().getDto()),
				std::list<RunTimeDTO>(RunTimeListSample4().getDtoList()),
				std::list<StartTimeDTO>(StartTimeListSample4().getDtoList())
			)
		) {}

		virtual std::shared_ptr<ProgramImpl::Builder> createBuilder() const override {
			auto builder = std::make_shared<ProgramImpl::Builder>();

			builder->
				setEnabled(true).
				setName("name4").
				setAdjustment(238).
				setSchedulerType(SchedulerType::WEEKLY).
				setSchedulerContainer(SchedulerContainerSample1().getObjectPtr()).
				setRunTimeContainer(RunTimeListSample4().getContainerPtr()).
				setStartTimeContainer(StartTimeListSample4().getContainerPtr());

			return builder;
		}
	};
};
