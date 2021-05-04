#pragma once
#include <memory>
#include "EmailConsumer.h"
#include "EmailProducer.h"

#define EMAIL EmailHandler::getInstance()


class EmailHandler {
	static std::unique_ptr<EmailHandler> instance;

	std::unique_ptr<EmailProducer> emailProducer;
	std::shared_ptr<EmailConsumer> emailConsumer;

	EmailHandler(
			const Email::Contact& from,
			const Email::Contact& to,
			const std::shared_ptr<EmailSender>& emailSender,
			const std::vector<std::chrono::milliseconds>& delayOnFailed
		);

	void start();
	void stop();

public:

	virtual ~EmailHandler();

	void send(EmailTopic topic, const std::string& messageText);

	void enableTopic(EmailTopic topic, bool enable = true);
	bool isTopicEnabled(EmailTopic topic) const;

	static void init(
			const Email::Contact& from,
			const Email::Contact& to,
			const std::shared_ptr<EmailSender>& emailSender,
			const std::vector<std::chrono::milliseconds>& delayOnFailed
		);

	static void uninit();
	static EmailHandler& getInstance();
};
