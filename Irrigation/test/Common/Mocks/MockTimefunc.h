#pragma once
#include <gmock/gmock.h>
#include "Utils/DateTime.h"


class MockTimefunc : public Timefunc {
public:
	MOCK_CONST_METHOD0(getTime, std::time_t());
};
