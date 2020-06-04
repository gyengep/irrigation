#pragma once
#include <map>
#include <memory>
#include <string>
#include "Utils/BlockingQueue.h"
#include "Utils/Thread.h"
#include "EmailSender.h"

#define EMAIL Email::getInstance()


enum class EmailTopic {
	WATERING_START,
	WATERING_SKIP,
	SYSTEM_STARTED,
	SYSTEM_STOPPED,
	TEST
};

class Email : public Thread {

	struct TopicProperties {
		bool enabled;
		std::string subject;

		TopicProperties(const std::string& subject);
	};

	static std::unique_ptr<Email> instance;

	const Contact from;
	const Contact to;

	mutable std::mutex mtx;
	std::map<EmailTopic, std::unique_ptr<TopicProperties>> topics;
	std::shared_ptr<EmailSender> emailSender;

	BlockingQueue<std::unique_ptr<Message>> messages;

	virtual void onExecute() override;

	TopicProperties& getTopicProperties(EmailTopic topic);
	const TopicProperties& getTopicProperties(EmailTopic topic) const ;

	Email(const std::shared_ptr<EmailSender>& emailSender);

public:
	virtual ~Email();

	void send(EmailTopic topic, const std::string& messageText);
	void stop();

	void enableTopic(EmailTopic topic, bool enable = true);
	bool isTopicEnabled(EmailTopic topic) const;

	static void init();
	static void init(const std::shared_ptr<EmailSender>& emailSender);
	static Email& getInstance();
};
