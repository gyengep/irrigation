#pragma once
#include <list>
#include <memory>
#include <string>
#include "RunTimeDTO.h"
#include "StartTimeDTO.h"
#include "SchedulersDTO.h"



class ProgramDTO {
	std::unique_ptr<const unsigned> id;
	std::unique_ptr<const std::string> name;
	std::unique_ptr<const std::string> schedulerType;
	std::unique_ptr<const SpecifiedSchedulerDTO> specifiedScheduler;
	std::unique_ptr<const std::list<RunTimeDTO>> runTimes;
	std::unique_ptr<const std::list<StartTimeDTO>> startTimes;

public:
	ProgramDTO() = default;
	ProgramDTO(ProgramDTO&& other) = default;
	ProgramDTO(const ProgramDTO& other);
	ProgramDTO(const std::string& name, const std::string& schedulerType,
			SpecifiedSchedulerDTO&& specifiedScheduler,
			std::list<RunTimeDTO>&& runTimes,
			std::list<StartTimeDTO>&& startTimes);

	ProgramDTO& operator= (ProgramDTO&& other) = delete;
	ProgramDTO& operator= (const ProgramDTO& other) = delete;

	bool operator== (const ProgramDTO& other) const;

	bool hasId() const;
	bool hasName() const;
	bool hasSchedulerType() const;
	bool hasSpecifiedScheduler() const;
	bool hasRunTimes() const;
	bool hasStartTimes() const;

	unsigned getId() const;
	const std::string& getName() const;
	const std::string& getSchedulerType() const;
	const SpecifiedSchedulerDTO& getSpecifiedScheduler() const;
	const std::list<RunTimeDTO>& getRunTimes() const;
	const std::list<StartTimeDTO>& getStartTimes() const;

	ProgramDTO& setId(unsigned id);
	ProgramDTO& setName(const std::string& name);
	ProgramDTO& setSchedulerType(const std::string& schedulerType);
	ProgramDTO& setSpecifiedScheduler(SpecifiedSchedulerDTO&& specifiedScheduler);
	ProgramDTO& setRunTimes(std::list<RunTimeDTO>&& runTimes);
	ProgramDTO& setStartTimes(std::list<StartTimeDTO>&& startTimes);

	friend std::ostream& operator<<(std::ostream& os, const ProgramDTO& program);
};
