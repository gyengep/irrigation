#pragma once
#include <gmock/gmock.h>
#include "Email/EmailHandler.h"
#include "Mocks/MockEmailSender.h"
#include "Mocks/MockWaitObserver.h"


class EmailHandlerTest : public ::testing::Test {
protected:

	std::unique_ptr<Email::Contact> from;
	std::unique_ptr<Email::Contact> to;
	std::shared_ptr<MockEmailSender> mockEmailSender;
	std::vector<std::chrono::milliseconds> waitTimes;
	std::shared_ptr<MockWaitObserver> mockWaitObserver;

	virtual void SetUp();
    virtual void TearDown();
};
