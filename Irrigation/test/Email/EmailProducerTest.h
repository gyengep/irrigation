#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Email/Email.h"
#include "Email/EmailProducer.h"


class EmailProducerTest : public ::testing::Test {
protected:

	std::unique_ptr<Email::Contact> from;
	std::unique_ptr<Email::Contact> to;
	std::shared_ptr<EmailQueue> emailQueue;
	std::unique_ptr<EmailProducer> emailProducer;

	virtual void SetUp();
    virtual void TearDown();
};
