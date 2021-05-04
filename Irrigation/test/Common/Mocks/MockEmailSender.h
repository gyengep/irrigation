#pragma once
#include <gmock/gmock.h>
#include "Email/EmailSender.h"


class MockEmailSender : public EmailSender {
public:
	MOCK_METHOD1(send, void(const Email& email));
};
