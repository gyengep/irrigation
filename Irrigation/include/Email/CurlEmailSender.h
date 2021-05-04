#pragma once
#include "EmailSender.h"


class CurlEmailSender : public EmailSender {
	const EmailSender::Properties properties;

public:
	CurlEmailSender(const EmailSender::Properties& properties);
	virtual ~CurlEmailSender();

	virtual void send(const Email& email) override;
};
