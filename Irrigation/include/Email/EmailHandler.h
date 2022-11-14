#pragma once
#include <memory>
#include "EmailConsumer.h"
#include "EmailProducer.h"


class EmailHandler {
	std::unique_ptr<EmailProducer> emailProducer;
	std::shared_ptr<EmailConsumer> emailConsumer;

public:
	EmailHandler(
			const Email::Contact& from,
			const Email::Contact& to,
			const std::shared_ptr<EmailSender>& emailSender,
			const std::vector<std::chrono::milliseconds>& delayOnFailed
		);

	virtual ~EmailHandler();

	void send(EmailTopic topic, const std::string& messageText);

	void enableTopic(EmailTopic topic, bool enable = true);
	bool isTopicEnabled(EmailTopic topic) const;

	void start();
	void stop();
};
