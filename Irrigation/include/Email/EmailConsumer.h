#pragma once
#include <memory>
#include "Utils/BlockingQueue.h"
#include "Utils/RepeatUntilSuccessRunnable.h"
#include "Utils/Runnable.h"
#include "Utils/SynchronizationObject.h"
#include "Utils/Thread.h"
#include "EmailSender.h"
#include "Email.h"

typedef std::shared_ptr<Email> EmailPtr;
typedef BlockingQueue<EmailPtr> EmailQueue;


class EmailConsumer : public Runnable {
	const std::vector<std::chrono::milliseconds> delayOnFailed;
	const std::shared_ptr<EmailQueue> emailQueue;
	const std::shared_ptr<EmailSender> emailSender;

	bool interrupted;

	std::shared_ptr<RepeatUntilSuccessRunnable> repeatUntilSuccessRunnable;

	std::unique_ptr<Thread> workerThread;
	SynchronizationObject synchronizationObject;

	virtual void run() override;
	virtual void interrupt() override;

	void safeSend(const EmailPtr& email);

public:
	EmailConsumer(
			const std::shared_ptr<EmailQueue>& emailQueue,
			const std::shared_ptr<EmailSender>& emailSender,
			const std::vector<std::chrono::milliseconds>& delayOnFailed
		);
	virtual ~EmailConsumer();

	void start();
	void stop();
};
