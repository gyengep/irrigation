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

	MOCK_CONST_METHOD0(toProgramDto, ProgramDTO());
	MOCK_METHOD1(updateFromProgramDto, void(const ProgramDTO& programDTO));

	MOCK_CONST_METHOD0(toString, std::string());
};

///////////////////////////////////////////////////////////////////////////////

class MockProgramFactory : public ProgramFactory {
	std::shared_ptr<MockProgram> getNext(unsigned) const {
		if (nextIndex >= mockPrograms.size()) {
			throw std::runtime_error("MockProgramFactory::getNext() invalid index: " + std::to_string(nextIndex));
		}

		return mockPrograms[nextIndex++];
	}

public:
	std::vector<std::shared_ptr<MockProgram>> mockPrograms;
	mutable size_t nextIndex;

	MockProgramFactory(size_t size) :
		mockPrograms(size),
		nextIndex(0)
	{
		for (size_t i = 0; i < mockPrograms.size(); ++i) {
			mockPrograms[i] = std::make_shared<testing::StrictMock<MockProgram>>();
		}

		ON_CALL(*this, create(testing::_)).WillByDefault(testing::Invoke(this, &MockProgramFactory::getNext));
	}

	MockProgramFactory(std::initializer_list<std::shared_ptr<MockProgram>> initializer) :
		mockPrograms(initializer),
		nextIndex(0)
	{
		ON_CALL(*this, create(testing::_)).WillByDefault(testing::Invoke(this, &MockProgramFactory::getNext));
	}

	MOCK_CONST_METHOD1(create, ProgramPtr(unsigned id));
};
