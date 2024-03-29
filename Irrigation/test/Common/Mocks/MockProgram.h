#pragma once
#include <vector>
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

	MOCK_METHOD1(isScheduled, std::unique_ptr<Scheduler::Result>(const LocalDateTime& localDateTime));


	MOCK_CONST_METHOD0(getSchedulerContainer, const SchedulerContainer& ());
	MOCK_CONST_METHOD0(getRunTimeContainer, const RunTimeContainer&());
	MOCK_CONST_METHOD0(getStartTimeContainer, const StartTimeContainer&());

	MOCK_METHOD0(getSchedulerContainer, SchedulerContainer& ());
	MOCK_METHOD0(getRunTimeContainer, RunTimeContainer&());
	MOCK_METHOD0(getStartTimeContainer, StartTimeContainer&());

	MOCK_CONST_METHOD0(toProgramDto, ProgramDto());
	MOCK_METHOD1(updateFromProgramDto, void(const ProgramDto& programDto));

	MOCK_CONST_METHOD0(toString, std::string());

	MOCK_CONST_METHOD0(saveTo, nlohmann::json());
	MOCK_METHOD1(loadFrom, void(const nlohmann::json& values));
};

///////////////////////////////////////////////////////////////////////////////

class MockProgramFactory : public ProgramFactory {
	unsigned idx = 0;

public:
	std::vector<std::shared_ptr<MockProgram>> mockPrograms;

	ProgramPtr createMockProgram() {
		if (mockPrograms.size() <= idx) {
			throw std::logic_error("MockProgramFactory::createMockProgram()");
		}

		return mockPrograms[idx++];
	}

	MOCK_CONST_METHOD0(create, ProgramPtr());
};
