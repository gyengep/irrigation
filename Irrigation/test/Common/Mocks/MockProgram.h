#pragma once
#include <gmock/gmock.h>
#include "Logic/Program.h"


class MockProgram : public Program {
public:
	MOCK_METHOD1(setEnabled, void(bool enabled));
	MOCK_METHOD1(setName, void(const std::string& name));
	MOCK_METHOD1(setAdjustment, void(unsigned adjustment));
	MOCK_METHOD1(setSchedulerType, void(SchedulerType schedulerType));

	MOCK_CONST_METHOD0(isEnabled, bool());
	MOCK_CONST_METHOD0(getName, const std::string&());
	MOCK_CONST_METHOD0(getAdjustment, unsigned());
	MOCK_CONST_METHOD0(getSchedulerType, SchedulerType());

	MOCK_METHOD1(isScheduled, std::unique_ptr<ScheduledResult>(const std::time_t rawtime));
	MOCK_METHOD0(getCurrentScheduler, Scheduler&());

	MOCK_CONST_METHOD0(getSchedulerContainer, const SchedulerContainer& ());
	MOCK_CONST_METHOD0(getRunTimeContainer, const RunTimeContainer&());
	MOCK_CONST_METHOD0(getStartTimeContainer, const StartTimeContainer&());

	MOCK_METHOD0(getSchedulerContainer, SchedulerContainer& ());
	MOCK_METHOD0(getRunTimeContainer, RunTimeContainer&());
	MOCK_METHOD0(getStartTimeContainer, StartTimeContainer&());

	MOCK_METHOD1(createStartTime, std::pair<IdType, StartTimePtr>(const StartTimeDTO& startTimeDto));

	MOCK_CONST_METHOD0(toProgramDto, ProgramDTO());
	MOCK_METHOD1(updateFromProgramDto, void(const ProgramDTO& programDTO));

	MOCK_CONST_METHOD0(toString, std::string());

	MOCK_CONST_METHOD0(saveTo, nlohmann::json());
	MOCK_METHOD1(loadFrom, void(const nlohmann::json& values));
};

///////////////////////////////////////////////////////////////////////////////

class MockProgramFactory : public ProgramFactory {
public:
	mutable std::vector<std::shared_ptr<MockProgram>> mockPrograms;
	mutable size_t index = 0;

	virtual ProgramPtr create() const {
		if (index >= mockPrograms.size()) {
			throw std::runtime_error("MockProgramFactory no more item");
		}

		return mockPrograms[index++];
	}
};
