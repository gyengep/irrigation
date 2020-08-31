#pragma once
#include "Exceptions/InterruptedException.h"
#include "BlockingQueue.h"
#include "IncrementalWait.h"
#include "Thread.h"


template <typename T>
class BlockingQueueThread : public Thread {
	BlockingQueue<T> queue;
	IncrementalWait incrementalWait;

	void itemAvailable(const T& value);

	virtual void onExecute() override;
	virtual void onItemAvailable(const T& value) = 0;
	virtual void onResumed() {}
	virtual void onError(size_t errorCount, const std::chrono::milliseconds&) {}

public:
	BlockingQueueThread(const std::string& name, const std::vector<std::chrono::milliseconds>& waitTimes);
	virtual ~BlockingQueueThread() = default;

	void push(const T& value);
	void push(T&& value);

	void stop();
};

template <typename T>
BlockingQueueThread<T>::BlockingQueueThread(const std::string& name, const std::vector<std::chrono::milliseconds>& waitTimes) : Thread(name),
	incrementalWait(waitTimes)
{
}

template <typename T>
void BlockingQueueThread<T>::push(const T& value) {
	queue.push(value);
}

template <typename T>
void BlockingQueueThread<T>::push(T&& value) {
	queue.push(std::move(value));
}

template <typename T>
void BlockingQueueThread<T>::stop() {
	queue.finish();
	incrementalWait.interrupt();
	join();
}

template <typename T>
void BlockingQueueThread<T>::onExecute() {
	try {

		while (queue.waitForElement()) {
			itemAvailable(queue.front());
			queue.pop();
		}

	} catch (const InterruptedException& e) {
	}
}

template <typename T>
void BlockingQueueThread<T>::itemAvailable(const T& value) {
	size_t errorCount = 0;

	while (true) {
		try {
			onItemAvailable(value);

			if (0 < errorCount) {
				errorCount = 0;
				incrementalWait.resetWaitTime();
				onResumed();
			}

			return;

		} catch (const std::exception& e) {
			errorCount++;
			onError(errorCount, incrementalWait.getWaitTime());
			incrementalWait.wait();
		}
	}

}
