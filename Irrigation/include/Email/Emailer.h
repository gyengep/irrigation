#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Utils/BlockingQueueThread.h"
#include "EmailSender.h"

#define EMAIL Emailer::getInstance()


enum class EmailTopic {
	WATERING_START,
	WATERING_SKIP,
	SYSTEM_STARTED,
	SYSTEM_STOPPED,
	TEST
};

class Emailer : public BlockingQueueThread<std::unique_ptr<Message>> {

	struct TopicProperties {
		bool enabled;
		std::string subject;

		TopicProperties(const std::string& subject);
	};

	const static std::vector<std::chrono::milliseconds> waitTimes;
	static std::unique_ptr<Emailer> instance;

	const Contact from;
	const Contact to;

	mutable std::mutex mtx;
	std::map<EmailTopic, std::unique_ptr<TopicProperties>> topics;
	std::shared_ptr<EmailSender> emailSender;

	bool sendingError;

	virtual void onItemAvailable(const std::unique_ptr<Message>& value) override;

	TopicProperties& getTopicProperties(EmailTopic topic);
	const TopicProperties& getTopicProperties(EmailTopic topic) const ;

	Emailer(const std::shared_ptr<EmailSender>& emailSender);

public:
	virtual ~Emailer();

	void send(EmailTopic topic, const std::string& messageText);

	void enableTopic(EmailTopic topic, bool enable = true);
	bool isTopicEnabled(EmailTopic topic) const;

	static void init(const std::shared_ptr<EmailSender>& emailSender);
	static void uninit();
	static Emailer& getInstance();
};
