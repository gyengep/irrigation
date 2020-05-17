#pragma once
#include <map>
#include <memory>
#include <string>

#define EMAIL Email::getInstance()


enum class EmailTopic {
	TEST,
	WATERING_START,
	WATERING_SKIP
};

class Email {

	struct TopicProperties {
		bool enabled;
		std::string subject;

		TopicProperties(const std::string& subject);
	};

	const std::string fromName;
	const std::string fromAddress;
	const std::string toName;
	const std::string toAddress;

	TopicProperties& getTopicProperties(EmailTopic topic);
	const TopicProperties& getTopicProperties(EmailTopic topic) const ;
	std::map<EmailTopic, std::unique_ptr<TopicProperties>> topics;

public:
	Email();
	virtual ~Email();

	void enableTopic(EmailTopic topic, bool enable = true);
	void send(EmailTopic topic, const std::string& message);

	bool isTopicEnabled(EmailTopic topic) const;

	static Email& getInstance();
};
