#pragma once
#include <gmock/gmock.h>
#include "Email/EmailConsumer.h"
#include "Mocks/MockEmailSender.h"
#include "Mocks/MockWaitObserver.h"


class EmailConsumerTest : public ::testing::Test {
protected:

	std::shared_ptr<EmailQueue> emailQueue;
	std::shared_ptr<MockEmailSender> mockEmailSender;
	std::vector<std::chrono::milliseconds> waitTimes;
	std::shared_ptr<MockWaitObserver> mockWaitObserver;

	std::shared_ptr<Email> email1;
	std::shared_ptr<Email> email2;
	std::shared_ptr<Email> email3;

	std::unique_ptr<EmailConsumer> emailConsumer;

	virtual void SetUp();
	virtual void TearDown();
};
