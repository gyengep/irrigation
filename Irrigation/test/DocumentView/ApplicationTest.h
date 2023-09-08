#pragma once
#include <gmock/gmock.h>
#include "DocumentView/Application.h"



class ApplicationTest : public ::testing::Test {
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

public:
    void waitAndExit(Application* application, unsigned waitMs);
};

