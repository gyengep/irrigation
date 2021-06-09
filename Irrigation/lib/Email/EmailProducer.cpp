#include "EmailProducer.h"


EmailProducer::TopicProperties::TopicProperties(const std::string& subject) :
	enabled(false),
	subject(subject)
{
}

///////////////////////////////////////////////////////////////////////////////

EmailProducer::EmailProducer(
	const std::shared_ptr<EmailQueue>& emailQueue,
	const Email::Contact& from,
	const Email::Contact& to
) :
	emailQueue(emailQueue),
	from(from),
	to(to)
{
	if (nullptr == emailQueue) {
		throw std::invalid_argument("EmailProducer::EmailProducer() nullptr == emailQueue");
	}

	topics[EmailTopic::WATERING_START].reset(new TopicProperties("Watering started"));
	topics[EmailTopic::WATERING_SKIP].reset(new TopicProperties("Watering skipped"));
	topics[EmailTopic::SYSTEM_STARTED].reset(new TopicProperties("System started"));
	topics[EmailTopic::SYSTEM_STOPPED].reset(new TopicProperties("System stopped"));
	topics[EmailTopic::TEST].reset(new TopicProperties("Test"));
}

EmailProducer::~EmailProducer() {
}

void EmailProducer::send(EmailTopic topic, const std::string& messageText) {
	std::unique_lock<std::mutex> lock(mtx);

	const auto& topicProperties = getTopicProperties(topic);

	if (topicProperties.enabled) {
		auto email = std::make_shared<Email>(
			from,
			std::list<Email::Contact>{to},
			std::list<Email::Contact>(),
			topicProperties.subject,
			messageText,
			DateTime::now()
		);

		emailQueue->push(email);
	}
}

void EmailProducer::enableTopic(EmailTopic topic, bool enable) {
	std::unique_lock<std::mutex> lock(mtx);
	getTopicProperties(topic).enabled = enable;
}

bool EmailProducer::isTopicEnabled(EmailTopic topic) const {
	std::unique_lock<std::mutex> lock(mtx);
	return getTopicProperties(topic).enabled;
}

///////////////////////////////////////////////////////////////////////////////

EmailProducer::TopicProperties& EmailProducer::getTopicProperties(EmailTopic topic) {
	const auto it = topics.find(topic);

	if (topics.end() == it) {
		throw std::logic_error("EmailProducer::send() topics.end() == it");
	}

	return *it->second;
}

const EmailProducer::TopicProperties& EmailProducer::getTopicProperties(EmailTopic topic) const {
	const auto it = topics.find(topic);

	if (topics.end() == it) {
		throw std::logic_error("EmailProducer::send() topics.end() == it");
	}

	return *it->second;
}

