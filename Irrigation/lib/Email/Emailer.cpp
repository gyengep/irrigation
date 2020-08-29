#include "Emailer.h"
#include "CurlEmailSender.h"
#include "Exceptions/InterruptedException.h"
#include "Logger/Logger.h"

using namespace std;


Emailer::TopicProperties::TopicProperties(const std::string& subject) :
	enabled(false),
	subject(subject)
{
}

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Emailer> Emailer::instance;

void Emailer::init(const std::shared_ptr<EmailSender>& emailSender) {
	instance.reset(new Emailer(emailSender));
}

void Emailer::uninit() {
	instance.reset();
}

Emailer& Emailer::getInstance() {
	if (nullptr == instance) {
		init(make_shared<CurlEmailSender>());
	}

	return *instance;
}

Emailer::Emailer(const std::shared_ptr<EmailSender>& emailSender) : Thread("Emailer"),
	from("Irrigation System", "irrigation.gyengep@gmail.com"),
	to("Gyenge Peter", "gyengep@gmail.com"),
	emailSender(emailSender),
	wait(vector<chrono::milliseconds>({
		chrono::minutes(1), chrono::minutes(2), chrono::minutes(5),
		chrono::minutes(15), chrono::minutes(30), chrono::minutes(60)
	}))
{
	if (nullptr == emailSender) {
		throw invalid_argument("Emailer::Emailer() nullptr == emailSender");
	}

	topics[EmailTopic::WATERING_START].reset(new TopicProperties("Watering started"));
	topics[EmailTopic::WATERING_SKIP].reset(new TopicProperties("Watering skipped"));
	topics[EmailTopic::SYSTEM_STARTED].reset(new TopicProperties("System started"));
	topics[EmailTopic::SYSTEM_STOPPED].reset(new TopicProperties("System stopped"));
	topics[EmailTopic::TEST].reset(new TopicProperties("Test"));
}

Emailer::~Emailer() {
}

void Emailer::stop() {
	messages.finish();
	wait.interrupt();
	join();
}

void Emailer::send(EmailTopic topic, const std::string& messageText) {
	unique_lock<mutex> lock(mtx);

	const auto& topicProperties = getTopicProperties(topic);

	if (topicProperties.enabled) {

		auto message = unique_ptr<Message>(new Message());
		message->from = from;
		message->to.push_back(to);
		message->subject = topicProperties.subject;
		message->text = messageText;
		message->date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		messages.push(move(message));
	}
}

void Emailer::enableTopic(EmailTopic topic, bool enable) {
	unique_lock<mutex> lock(mtx);
	getTopicProperties(topic).enabled = enable;
}

bool Emailer::isTopicEnabled(EmailTopic topic) const {
	unique_lock<mutex> lock(mtx);
	return getTopicProperties(topic).enabled;
}

///////////////////////////////////////////////////////////////////////////////

Emailer::TopicProperties& Emailer::getTopicProperties(EmailTopic topic) {
	const auto it = topics.find(topic);

	if (topics.end() == it) {
		throw std::logic_error("Emailer::send() topics.end() == it");
	}

	return *it->second;
}

const Emailer::TopicProperties& Emailer::getTopicProperties(EmailTopic topic) const {
	const auto it = topics.find(topic);

	if (topics.end() == it) {
		throw std::logic_error("Emailer::send() topics.end() == it");
	}

	return *it->second;
}

///////////////////////////////////////////////////////////////////////////////

void Emailer::onExecute() {
	bool error = false;

	try {

		while (messages.waitForElement()) {
			try {
				emailSender->send(*messages.front());
				messages.pop();
				wait.resetWaitTime();

				if (true == error) {
					LOGGER.info("Email sending resumed");
				}

				error = false;
			} catch (const std::exception& e) {
				LOGGER.warning("Email sending failed", e);
				wait.wait();
				wait.incrementWaitTime();
				error = true;
			}
		}

	} catch (const InterruptedException& e) {
		LOGGER.warning("There are some unsent email in the queue");
	}
}
