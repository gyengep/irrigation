#include <gmock/gmock.h>
#include <memory>
#include "Logic/RunTime.h"



class MockRunTime : public RunTime {
public:
	MockRunTime() {
		EXPECT_CALL(*this, destroyed()).Times(1);
	}

	MOCK_METHOD0(destroyed, void());
	virtual ~MockRunTime() { destroyed(); }
};

///////////////////////////////////////////////////////////////////////////////

class MockRunTimeFactory : public RunTimeFactory {
public:
	virtual std::shared_ptr<RunTime> createRunTime() const {
		return std::shared_ptr<RunTime>(new MockRunTime());
	}
};

