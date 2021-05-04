#include <gmock/gmock.h>
#include "EmailHandlerTest.h"
#include "Utils/WaitObserverStore.h"

using namespace std;
using namespace testing;


void EmailHandlerTest::SetUp() {
	waitTimes = std::vector<std::chrono::milliseconds>{
		std::chrono::milliseconds(10),
		std::chrono::milliseconds(20),
		std::chrono::milliseconds(30)
	};

	from = std::unique_ptr<Email::Contact>(new Email::Contact("EmailSender", "sender@abc.hu"));
	to = std::unique_ptr<Email::Contact>(new Email::Contact("EmailReceiver", "receiver@abc.hu"));
	mockEmailSender = std::make_shared<MockEmailSender>();
	mockWaitObserver = std::make_shared<MockWaitObserver>();

	WaitObserverStore::getInstance().insert("EmailConsumerSync", mockWaitObserver);

	EmailHandler::init(
			*from.get(),
			*to.get(),
			mockEmailSender,
			waitTimes
		);
}

void EmailHandlerTest::TearDown() {
	EmailHandler::uninit();
	WaitObserverStore::getInstance().clear();
}


TEST_F(EmailHandlerTest, sendEnabled) {
	EXPECT_CALL(*mockEmailSender, send(_)).Times(1);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(0);

	EMAIL.enableTopic(EmailTopic::TEST, true);

	EMAIL.send(EmailTopic::WATERING_START, "Message Body");
	EMAIL.send(EmailTopic::WATERING_SKIP, "Message Body");
	EMAIL.send(EmailTopic::SYSTEM_STARTED, "Message Body");
	EMAIL.send(EmailTopic::SYSTEM_STOPPED, "Message Body");
	EMAIL.send(EmailTopic::TEST, "Message Body");
}

TEST_F(EmailHandlerTest, sendDisabled) {
	EXPECT_CALL(*mockEmailSender, send(_)).Times(0);

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(_)).Times(0);

	EMAIL.enableTopic(EmailTopic::TEST, false);

	EMAIL.send(EmailTopic::WATERING_START, "Message Body");
	EMAIL.send(EmailTopic::WATERING_SKIP, "Message Body");
	EMAIL.send(EmailTopic::SYSTEM_STARTED, "Message Body");
	EMAIL.send(EmailTopic::SYSTEM_STOPPED, "Message Body");
	EMAIL.send(EmailTopic::TEST, "Message Body");
}

TEST_F(EmailHandlerTest, sendFailed) {
	EXPECT_CALL(*mockEmailSender, send(_)).
			Times(2).
			WillOnce(Throw(std::runtime_error(""))).
			WillOnce(Return());

	EXPECT_CALL(*mockWaitObserver, wait()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for(_)).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_pred()).Times(0);
	EXPECT_CALL(*mockWaitObserver, wait_for_pred(waitTimes.front())).Times(1);

	EMAIL.enableTopic(EmailTopic::TEST, true);
	EMAIL.send(EmailTopic::TEST, "Message Body");

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
