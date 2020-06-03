#include "Email.h"
#include "CurlEmailSender.h"
#include "CurlStringReader.h"
#include "Logger/Logger.h"


#include <iostream>

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

Email::Email() : Thread("Email"),
	from("Irrigation System", "irrigation.gyengep@gmail.com"),
	to("Gyenge Peter", "gyengep@gmail.com")
{
	topics[EmailTopic::WATERING_START].reset(new TopicProperties("Watering started"));
	topics[EmailTopic::WATERING_SKIP].reset(new TopicProperties("Watering skipped"));
	topics[EmailTopic::SYSTEM_STARTED].reset(new TopicProperties("System started"));
	topics[EmailTopic::SYSTEM_STOPPED].reset(new TopicProperties("System stopped"));
	topics[EmailTopic::TEST].reset(new TopicProperties("Test"));
}

Email::~Email() {
}

void Email::stop() {
	messages.finish();
	join();
}

void Email::send(EmailTopic topic, const std::string& messageText) {
	unique_lock<mutex> lock(mtx);

	const auto& topicProperties = getTopicProperties(topic);

	if (topicProperties.enabled) {

		auto message = unique_ptr<Message>(new Message());
		message->from = from;
		message->to.push_back(to);
		message->subject = topicProperties.subject;
		message->text = messageText;

		messages.push(move(message));
	}
}

void Email::enableTopic(EmailTopic topic, bool enable) {
	unique_lock<mutex> lock(mtx);
	getTopicProperties(topic).enabled = enable;
}

bool Email::isTopicEnabled(EmailTopic topic) const {
	unique_lock<mutex> lock(mtx);
	return getTopicProperties(topic).enabled;
}

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

void Email::onExecute() {
	LOGGER.trace("Email::onExecute() __BEGIN__");

	while (messages.waitForElement()) {
		LOGGER.trace("Email::onExecute() hasNext()");
		CurlEmailSender().send(*messages.front());
		messages.pop();
	}

	LOGGER.trace("Email::onExecute() __END__");
}
