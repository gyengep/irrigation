#pragma once
#include "EmailSender.h"


class CurlEmailSender : public EmailSender {
	const std::string url;
	const std::string username;
	const std::string password;

public:
	CurlEmailSender(const std::string& url, const std::string& username, const std::string& password);
	virtual ~CurlEmailSender();

	virtual void send(const Email& email) override;
};
