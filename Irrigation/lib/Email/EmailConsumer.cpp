#include "EmailConsumer.h"
#include "Exceptions/InterruptedException.h"
#include "Logger/Logger.h"
#include "Utils/FunctionRunnable.h"
#include "Utils/TimeContainer.h"
#include "Utils/TimeConversion.h"
#include <sstream>


EmailConsumer::EmailConsumer(
		const std::shared_ptr<EmailQueue>& emailQueue,
		const std::shared_ptr<EmailSender>& emailSender,
		const std::vector<std::chrono::milliseconds>& delayOnFailed
	)
:
	Thread("EmailConsumer"),
	delayOnFailed(delayOnFailed),
	emailQueue(emailQueue),
	emailSender(emailSender),
	synchronizationObject()
{
	if (nullptr == emailQueue) {
		throw std::invalid_argument("EmailConsumer::EmailConsumer() nullptr == emailQueue");
	}

	if (nullptr == emailSender) {
		throw std::invalid_argument("EmailConsumer::EmailConsumer() nullptr == emailSender");
	}

	if (delayOnFailed.empty()) {
		throw std::invalid_argument("EmailConsumer::EmailConsumer() delayOnFailed is empty");
	}
}

EmailConsumer::~EmailConsumer() {
}

void EmailConsumer::run() {
	try {
		while(emailQueue->waitForElement()) {
			safeSend(emailQueue->front());
			emailQueue->pop();
		}
	} catch (const InterruptedException&) {
		LOGGER.debug("EmailConsumer is interrupted");
	}
}

void EmailConsumer::interrupt() {
	auto lock = synchronizationObject.synchronize();

	emailQueue->interrupt();

	if (repeatUntilSuccessRunnable) {
		repeatUntilSuccessRunnable->interrupt();
	}
}

void EmailConsumer::safeSend(const EmailPtr& email) {
	auto lock = synchronizationObject.synchronize();

	auto sendEmail = [this, &email] () {
		emailSender->send(*email);
	};

	repeatUntilSuccessRunnable = std::make_shared<RepeatUntilSuccessRunnable>(
			std::make_shared<FunctionRunnable>(sendEmail),
			delayOnFailed,
			"Email sending"
		);

	repeatUntilSuccessRunnable->setSynchronizationObjectName("EmailConsumerSync");
	lock->unlock();

	repeatUntilSuccessRunnable->run();

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		std::ostringstream oss;

		oss << "Email successfully sent to '";
		oss << Email::Contact::toString(email->to);
		oss << "' in topic: '" << email->subject << "'";

		LOGGER.debug(oss.str().c_str());
	}

	lock->lock();
	repeatUntilSuccessRunnable.reset();
}
