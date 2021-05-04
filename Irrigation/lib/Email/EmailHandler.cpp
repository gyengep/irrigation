#include "EmailHandler.h"
#include "EmailSender.h"
#include "Utils/BlockingQueue.h"


std::unique_ptr<EmailHandler> EmailHandler::instance;


void EmailHandler::init(
		const Email::Contact& from,
		const Email::Contact& to,
		const std::shared_ptr<EmailSender>& emailSender,
		const std::vector<std::chrono::milliseconds>& delayOnFailed
) {
	instance.reset(new EmailHandler(
			from,
			to,
			emailSender,
			delayOnFailed
		));

	instance->start();
}

void EmailHandler::uninit() {
	instance->stop();
	instance.reset();
}

EmailHandler& EmailHandler::getInstance() {
	if (nullptr == instance) {
		throw std::logic_error("EmailHandler is not initialized");
	}

	return *instance;
}

///////////////////////////////////////////////////////////////////////////////

EmailHandler::EmailHandler(
		const Email::Contact& from,
		const Email::Contact& to,
		const std::shared_ptr<EmailSender>& emailSender,
		const std::vector<std::chrono::milliseconds>& delayOnFailed
	)
{
	std::shared_ptr<EmailQueue> emailQueue = std::make_shared<EmailQueue>();

	emailProducer = std::unique_ptr<EmailProducer>(new EmailProducer(emailQueue, from, to));
	emailConsumer = std::unique_ptr<EmailConsumer>(new EmailConsumer(emailQueue, emailSender, delayOnFailed));
}

EmailHandler::~EmailHandler() {
}

void EmailHandler::start() {
	emailConsumer->start();
}

void EmailHandler::stop() {
	emailConsumer->stop();
}

void EmailHandler::send(EmailTopic topic, const std::string& messageText) {
	emailProducer->send(topic, messageText);
}

void EmailHandler::enableTopic(EmailTopic topic, bool enable) {
	emailProducer->enableTopic(topic, enable);
}

bool EmailHandler::isTopicEnabled(EmailTopic topic) const {
	return emailProducer->isTopicEnabled(topic);
}

