#include <gmock/gmock.h>
#include <thread>
#include "Exceptions/InterruptedException.h"
#include "Utils/WaitObserverStore.h"
#include "EmailConsumerTest.h"
#include "TestCommon/cout.h"

using namespace std;
using namespace testing;


void EmailConsumerTest::SetUp() {
	waitTimes = std::vector<std::chrono::milliseconds>{
		std::chrono::milliseconds(10),
		std::chrono::milliseconds(20),
		std::chrono::milliseconds(30)
	};

	emailQueue = std::make_shared<EmailQueue>();
	mockEmailSender = std::make_shared<MockEmailSender>();
	mockWaitObserver = std::make_shared<MockWaitObserver>();

	WaitObserverStore::getInstance().insert("EmailConsumerSync", mockWaitObserver);

	emailConsumer = std::unique_ptr<EmailConsumer>(new EmailConsumer(
			emailQueue,
			mockEmailSender,
			waitTimes
		));


	email1 = std::make_shared<Email>(
		Email::Contact("name11", "address11"),
		std::list<Email::Contact>{Email::Contact("name12", "address12")},
		std::list<Email::Contact>{Email::Contact("name13", "address13")},
		"mySubject1",
		"myMessage1",
		std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())
	);

	email2 = std::make_shared<Email>(
		Email::Contact("name21", "address21"),
		std::list<Email::Contact>{Email::Contact("name22", "address22")},
		std::list<Email::Contact>{Email::Contact("name23", "address23")},
		"mySubject2",
		"myMessage2",
		std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())
	);

	email3 = std::make_shared<Email>(
		Email::Contact("name31", "address31"),
		std::list<Email::Contact>{Email::Contact("name32", "address32")},
		std::list<Email::Contact>{Email::Contact("name33", "address33")},
		"mySubject3",
		"myMessage3",
		std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())
	);

}

void EmailConsumerTest::TearDown() {
	WaitObserverStore::getInstance().clear();
}


TEST_F(EmailConsumerTest, sendSuccess_TIMING) {
	EXPECT_CALL(*mockEmailSender, send(*email1)).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(0);

	emailConsumer->start();
	emailQueue->push(email1);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	emailConsumer->stop();
}

TEST_F(EmailConsumerTest, send3Times_TIMING) {
	EXPECT_CALL(*mockEmailSender, send(*email1)).Times(1);
	EXPECT_CALL(*mockEmailSender, send(*email2)).Times(1);
	EXPECT_CALL(*mockEmailSender, send(*email3)).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(0);

	emailConsumer->start();
	emailQueue->push(email1);
	emailQueue->push(email2);
	emailQueue->push(email3);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	emailConsumer->stop();
}

TEST_F(EmailConsumerTest, sendFailed_TIMING) {
	EXPECT_CALL(*mockEmailSender, send(*email2)).
			Times(AtLeast(5)).
			WillRepeatedly(Throw(std::runtime_error("")));

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[0])).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[1])).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[2])).Times(AtLeast(3));

	emailConsumer->start();
	emailQueue->push(email2);

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	emailConsumer->stop();
}

TEST_F(EmailConsumerTest, sendFailedAndResumed_TIMING) {
	EXPECT_CALL(*mockEmailSender, send(*email3)).
			Times(5).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Return());

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[0])).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[1])).Times(1);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[2])).Times(2);

	emailConsumer->start();
	emailQueue->push(email3);

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	emailConsumer->stop();
}

TEST_F(EmailConsumerTest, sendFailedAndResumedAndSend_TIMING) {
	EXPECT_CALL(*mockEmailSender, send(*email1)).
			Times(3).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Return());

	EXPECT_CALL(*mockEmailSender, send(*email2)).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Return());

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[0])).Times(2);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[1])).Times(2);

	emailConsumer->start();
	emailQueue->push(email1);
	emailQueue->push(email2);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	emailConsumer->stop();
}

TEST_F(EmailConsumerTest, stopDuringSendingSuccess_TIMING) {
	auto wait500 = [] {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	};

	EXPECT_CALL(*mockEmailSender, send(*email1)).
			Times(1).
			WillRepeatedly(InvokeWithoutArgs(wait500));

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(0);

	emailConsumer->start();
	emailQueue->push(email1);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	emailConsumer->stop();
}

TEST_F(EmailConsumerTest, stopDuringSendingFailed_TIMING) {
	auto wait500 = [] {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	};

	EXPECT_CALL(*mockEmailSender, send(*email1)).
			Times(1).
			WillRepeatedly(DoAll(InvokeWithoutArgs(wait500), Throw(std::runtime_error(""))));

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes[0])).Times(1);

	emailConsumer->start();
	emailQueue->push(email1);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	emailConsumer->stop();
}

TEST_F(EmailConsumerTest, stopDuringWaiting_TIMING) {
	EXPECT_CALL(*mockEmailSender, send(*email1)).
			Times(1).
			WillRepeatedly(Throw(std::runtime_error("")));

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(std::chrono::milliseconds(1000))).Times(1);

	emailConsumer = std::unique_ptr<EmailConsumer>(new EmailConsumer(
			emailQueue,
			mockEmailSender,
			std::vector<std::chrono::milliseconds>{ std::chrono::milliseconds(1000) }
		));

	emailConsumer->start();
	emailQueue->push(email1);

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	emailConsumer->stop();
}

