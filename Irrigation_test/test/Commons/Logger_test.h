#pragma once


class LoggerTest : public ::testing::Test {
protected:

	Logger::Level levelSave;

    virtual void SetUp();
    virtual void TearDown();
};

