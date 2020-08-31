#include "Emailer.h"
#include "CurlEmailSender.h"
#include "Exceptions/InterruptedException.h"
#include "Logger/Logger.h"
#include "Utils/IncrementalWait.h"
#include "Utils/TimeConversion.h"

using namespace std;


Emailer::TopicProperties::TopicProperties(const std::string& subject) :
	enabled(false),
	subject(subject)
{
}

///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Emailer> Emailer::instance;
const std::vector<std::chrono::milliseconds> Emailer::waitTimes {
	chrono::minutes(1), chrono::minutes(2), chrono::minutes(5),
	chrono::minutes(15), chrono::minutes(30), chrono::minutes(60)
};


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

Emailer::Emailer(const std::shared_ptr<EmailSender>& emailSender) : BlockingQueueThread<std::unique_ptr<Message>>("Emailer", waitTimes),
	from("Irrigation System", "irrigation.gyengep@gmail.com"),
	to("Gyenge Peter", "gyengep@gmail.com"),
	emailSender(emailSender)
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

		push(move(message));
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

void Emailer::onItemAvailable(const std::unique_ptr<Message>& message) {
	emailSender->send(*message);
}

void Emailer::onResumed() {
	LOGGER.info("Email sending resumed");
}

void Emailer::onError(size_t errorCount, const std::chrono::milliseconds& waitTime) {
	if (LOGGER.isLoggable(LogLevel::WARNING)) {
		const auto nextTryTime = std::chrono::system_clock::now() + waitTime;
		const std::string nextTryTimeStr = toLocalTimeStr(std::chrono::system_clock::to_time_t(nextTryTime), "%T");
		LOGGER.warning("Email sending failed on try %u, next try at %s", errorCount, nextTryTimeStr.c_str());
	}
}
