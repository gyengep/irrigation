#pragma once
#include <gmock/gmock.h>
#include "Model/Application.h"



class MockApplication : public Application {
public:
	MOCK_METHOD0(onInitialize, void());
	MOCK_METHOD0(onTerminate, void());
};


class ApplicationTest : public ::testing::Test {
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

public:
    void waitAndExit(Application* application, unsigned waitMs);
};

