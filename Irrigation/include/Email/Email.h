#pragma once
#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include "Utils/BlockingQueue.h"
#include "Utils/Thread.h"

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

	const std::string fromName;
	const std::string fromAddress;
	const std::string toName;
	const std::string toAddress;

	mutable std::mutex mtx;
	BlockingQueue<std::pair<std::string, std::string>> messages;

	virtual void onExecute() override;

	TopicProperties& getTopicProperties(EmailTopic topic);
	const TopicProperties& getTopicProperties(EmailTopic topic) const ;
	std::map<EmailTopic, std::unique_ptr<TopicProperties>> topics;

public:
	Email();
	virtual ~Email();

	void send(EmailTopic topic, const std::string& message);
	void stop();

	void enableTopic(EmailTopic topic, bool enable = true);
	bool isTopicEnabled(EmailTopic topic) const;

	static Email& getInstance();
};
