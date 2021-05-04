#pragma once
#include <map>
#include <memory>
#include <mutex>
#include "Utils/BlockingQueue.h"
#include "Email.h"

typedef std::shared_ptr<Email> EmailPtr;
typedef BlockingQueue<EmailPtr> EmailQueue;


enum class EmailTopic {
	WATERING_START,
	WATERING_SKIP,
	SYSTEM_STARTED,
	SYSTEM_STOPPED,
	TEST
};

class EmailProducer {
	const std::shared_ptr<EmailQueue> emailQueue;
	const Email::Contact from;
	const Email::Contact to;

	struct TopicProperties {
		bool enabled;
		std::string subject;

		TopicProperties(const std::string& subject);
	};

	mutable std::mutex mtx;
	std::map<EmailTopic, std::unique_ptr<TopicProperties>> topics;

	TopicProperties& getTopicProperties(EmailTopic topic);
	const TopicProperties& getTopicProperties(EmailTopic topic) const ;

public:
	EmailProducer(
			const std::shared_ptr<EmailQueue>& emailQueue,
			const Email::Contact& from,
			const Email::Contact& to
		);
	virtual ~EmailProducer();

	void send(EmailTopic topic, const std::string& messageText);

	void enableTopic(EmailTopic topic, bool enable);
	bool isTopicEnabled(EmailTopic topic) const;
};
