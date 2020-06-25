#include <gmock/gmock.h>
#include <fstream>
#include "Exceptions/Exceptions.h"
#include "Email/Email.h"

using namespace std;
using ::testing::_;


class MockEmailSender : public EmailSender {
public:
	MOCK_METHOD1(send, void(const Message& message));
};


TEST(EmailTest, send) {
	EMAIL.start();
	//EMAIL.enableTopic(EmailTopic::TEST);
	EMAIL.send(EmailTopic::TEST, "Message Body");
	EMAIL.stop();
}

TEST(EmailTest, enable) {
	Email::init(make_shared<MockEmailSender>());

	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::TEST));

	EMAIL.enableTopic(EmailTopic::WATERING_START);

	EXPECT_TRUE(EMAIL.isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::TEST));

	EMAIL.enableTopic(EmailTopic::WATERING_START, false);

	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(EMAIL.isTopicEnabled(EmailTopic::TEST));

	Email::uninit();
}

TEST(EmailTest, topicEnabled) {
	auto mockEmailSender = make_shared<MockEmailSender>();

	EXPECT_CALL(*mockEmailSender, send(_)).Times(1);

	Email::init(mockEmailSender);
	EMAIL.start();
	EMAIL.enableTopic(EmailTopic::TEST);
	EMAIL.send(EmailTopic::WATERING_START, "Message Body");
	EMAIL.send(EmailTopic::WATERING_SKIP, "Message Body");
	EMAIL.send(EmailTopic::SYSTEM_STARTED, "Message Body");
	EMAIL.send(EmailTopic::SYSTEM_STOPPED, "Message Body");
	EMAIL.send(EmailTopic::TEST, "Message Body");
	EMAIL.stop();
	Email::uninit();
}
