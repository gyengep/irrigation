#include "Email.h"
#include "EmailSender.h"
#include "CurlStringReader.h"
#include "Logger/Logger.h"

using namespace std;


Email::TopicProperties::TopicProperties(const std::string& subject) :
	enabled(false),
	subject(subject)
{
}

///////////////////////////////////////////////////////////////////////////////

Email& Email::getInstance() {
	static Email instance;
	return instance;
}

Email::Email() :
	fromName("Irrigation System"),
	fromAddress("irrigation.gyengep@gmail.com"),
	toName("Gyenge Peter"),
	toAddress("gyengep@gmail.com")
{
	topics[EmailTopic::TEST].reset(new TopicProperties("Test"));
	topics[EmailTopic::WATERING_START].reset(new TopicProperties("Watering started"));
	topics[EmailTopic::WATERING_SKIP].reset(new TopicProperties("Watering skipped"));
}

Email::~Email() {
}

Email::TopicProperties& Email::getTopicProperties(EmailTopic topic) {
	const auto it = topics.find(topic);

	if (topics.end() == it) {
		throw std::logic_error("Email::send() topics.end() == it");
	}

	return *it->second;
}

const Email::TopicProperties& Email::getTopicProperties(EmailTopic topic) const {
	const auto it = topics.find(topic);

	if (topics.end() == it) {
		throw std::logic_error("Email::send() topics.end() == it");
	}

	return *it->second;
}

void Email::send(EmailTopic topic, const std::string& message) {
	const auto& topicProperties = getTopicProperties(topic);

	if (topicProperties.enabled) {
		const EmailSender::Person from(fromName, fromAddress);
		const EmailSender::Person to(toName, toAddress);

		EmailSender emailSender(from, std::list<EmailSender::Person>{ to }, std::list<EmailSender::Person>());
		emailSender.send(topicProperties.subject, message);
	}
}

void Email::enableTopic(EmailTopic topic, bool enable) {
	getTopicProperties(topic).enabled = enable;
}

bool Email::isTopicEnabled(EmailTopic topic) const {
	return getTopicProperties(topic).enabled;

}
