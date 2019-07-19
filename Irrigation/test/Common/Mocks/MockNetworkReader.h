#pragma once
#include <gmock/gmock.h>
#include "Hardware/Temperature/NetworkReader.h"


class MockNetworkReader : public NetworkReader {
public:
	MOCK_CONST_METHOD1(read, std::string(const std::string& url));
};

