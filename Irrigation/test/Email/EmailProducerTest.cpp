#include "EmailProducerTest.h"

using namespace std;
using namespace testing;


void EmailProducerTest::SetUp() {

	from = std::unique_ptr<Email::Contact>(new Email::Contact("EmailSender", "sender@abc.hu"));
	to = std::unique_ptr<Email::Contact>(new Email::Contact("EmailReceiver", "receiver@abc.hu"));
	emailQueue = std::make_shared<EmailQueue>();

	emailProducer = std::unique_ptr<EmailProducer>(new EmailProducer(
			emailQueue,
			*from,
			*to
		));
}

void EmailProducerTest::TearDown() {
}


TEST_F(EmailProducerTest, init) {
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::TEST));
}

TEST_F(EmailProducerTest, enableTopic) {
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::TEST));

	emailProducer->enableTopic(EmailTopic::WATERING_START, true);

	EXPECT_TRUE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::TEST));

	emailProducer->enableTopic(EmailTopic::WATERING_START, false);

	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::TEST));

	emailProducer->enableTopic(EmailTopic::WATERING_SKIP, true);

	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_TRUE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::TEST));

	emailProducer->enableTopic(EmailTopic::WATERING_SKIP, false);

	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::TEST));

	emailProducer->enableTopic(EmailTopic::SYSTEM_STARTED, true);

	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_TRUE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::TEST));

	emailProducer->enableTopic(EmailTopic::SYSTEM_STARTED, false);

	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::TEST));

	emailProducer->enableTopic(EmailTopic::SYSTEM_STOPPED, true);

	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_TRUE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::TEST));

	emailProducer->enableTopic(EmailTopic::SYSTEM_STOPPED, false);

	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::TEST));

	emailProducer->enableTopic(EmailTopic::TEST, true);

	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_TRUE(emailProducer->isTopicEnabled(EmailTopic::TEST));

	emailProducer->enableTopic(EmailTopic::TEST, false);

	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_START));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::WATERING_SKIP));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STARTED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::SYSTEM_STOPPED));
	EXPECT_FALSE(emailProducer->isTopicEnabled(EmailTopic::TEST));
}

TEST_F(EmailProducerTest, sendDisabled) {
	emailProducer->send(EmailTopic::TEST, "TestMessage");
	EXPECT_THAT(emailQueue->size(), Eq(0));
}

TEST_F(EmailProducerTest, sendEnabled) {
	const std::string message = "TestMessage";

	emailProducer->enableTopic(EmailTopic::TEST, true);
	emailProducer->send(EmailTopic::TEST, message);
	ASSERT_THAT(emailQueue->size(), Eq(1));
	EXPECT_THAT(emailQueue->front()->from, Eq(*from));
	EXPECT_THAT(emailQueue->front()->to, ContainerEq(std::list<Email::Contact>{*to}));
	EXPECT_THAT(emailQueue->front()->cc, IsEmpty());
	EXPECT_THAT(emailQueue->front()->subject, Eq("Test"));
	EXPECT_THAT(emailQueue->front()->text, Eq(message));
}

