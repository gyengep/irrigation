#pragma once
#include <memory>
#include <string>
#include "Email.h"


class EmailSender {
public:

	struct Properties;

	virtual ~EmailSender() = default;
	virtual void send(const Email& email) = 0;

	static std::shared_ptr<EmailSender> create(const Properties& properties);
};


struct EmailSender::Properties {
	std::string url;
	std::string username;
	std::string password;

	Properties(const std::string& url, const std::string& username, const std::string& password);
};
