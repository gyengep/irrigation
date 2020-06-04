#pragma once
#include <string>
#include "EmailSender.h"


class CurlEmailSender : public EmailSender {
	const std::string url;
	const std::string username;
	const std::string password;

public:
	CurlEmailSender();
	virtual ~CurlEmailSender();

	virtual void send(const Message& message) override;
};
