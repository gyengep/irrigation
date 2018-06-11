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
	ProgramDTO();
	ProgramDTO(const ProgramDTO& other);
	ProgramDTO(const std::string& name, const std::string& schedulerType,
			const SpecifiedSchedulerDTO& specifiedScheduler,
			const std::list<RunTimeDTO>* runTimes,
			const std::list<StartTimeDTO>* startTimes);

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
	ProgramDTO& setName(const char* name);
	ProgramDTO& setSchedulerType(const char* schedulerType);
	ProgramDTO& setSpecifiedScheduler(const SpecifiedSchedulerDTO& specifiedScheduler);
	ProgramDTO& setRunTimes(const std::list<RunTimeDTO>* runTimes);
	ProgramDTO& setStartTimes(const std::list<StartTimeDTO>* startTimes);

	friend std::ostream& operator<<(std::ostream& os, const ProgramDTO& program);
};
