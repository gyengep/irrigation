#pragma once
#include <memory>
#include <string>
#include "Email.h"


class EmailSender {
public:

	virtual ~EmailSender() = default;
	virtual void send(const Email& email) = 0;

	static std::shared_ptr<EmailSender> create();
};
