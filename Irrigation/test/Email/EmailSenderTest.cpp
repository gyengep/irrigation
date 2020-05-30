#include <gmock/gmock.h>
#include "Email/EmailSender.h"

using namespace std;
using namespace testing;


TEST(EmailSenderPersonTest, person) {
	const std::string name("abcd");
	const std::string address("123456");

	EmailSender::Person person(name, address);

	EXPECT_THAT(person.name, Eq(name));
	EXPECT_THAT(person.address, Eq(address));
}

TEST(EmailSenderPersonTest, personWithoutName) {
	const std::string name;
	const std::string address("123456");

	EmailSender::Person person(name, address);

	EXPECT_THAT(person.name, Eq(name));
	EXPECT_THAT(person.address, Eq(address));
}

TEST(EmailSenderPersonTest, personWithoutAddress) {
	const std::string name("abcd");
	const std::string address;

	EXPECT_THROW(EmailSender::Person(name, address), std::runtime_error);
}

TEST(EmailSenderPersonTest, personToString) {
	EXPECT_THAT(EmailSender::Person("abcd", "123456").toString(), Eq("abcd <123456>"));
	EXPECT_THAT(EmailSender::Person("", "123456").toString(), Eq("123456"));
}

TEST(EmailSenderPersonTest, personListToString) {
	std::list<EmailSender::Person> persons {
		EmailSender::Person("abcd", "123456"),
		EmailSender::Person("xyz", "987")
	};

	EXPECT_THAT(EmailSender::Person::toString(persons), Eq("abcd <123456>, xyz <987>"));
}

TEST(EmailSenderPersonTest, emptyPersonListToString) {
	EXPECT_THROW(EmailSender::Person::toString(std::list<EmailSender::Person>()), std::runtime_error);
}

///////////////////////////////////////////////////////////////////////////////

TEST(EmailSenderTest, construct) {

	EmailSender::Person person1("qwert1", "1_9876543");
	EmailSender::Person person2("qwert2", "2_9876543");
	EmailSender::Person person3("qwert3", "3_9876543");

	EXPECT_NO_THROW(EmailSender emailSender(
			person1,
			std::list<EmailSender::Person>{ person2 },
			std::list<EmailSender::Person>{ person3 }
		));

	EXPECT_NO_THROW(EmailSender emailSender(
			person1,
			std::list<EmailSender::Person>{ person2 },
			std::list<EmailSender::Person>()
		));

	EXPECT_THROW(EmailSender emailSender(
			person1,
			std::list<EmailSender::Person>(),
			std::list<EmailSender::Person>{ person3 }
		), std::runtime_error);
}

TEST(EmailSenderTest, createFullMessage1) {
	const std::string expectedMessage =
			"From: abcdef <12345678>\r\n"
			"To: qwert <9876543>\r\n"
			"Subject: MySubject\r\n"
			"\r\n"
			"MyMessage";

	EmailSender::Person from("abcdef", "12345678");
	std::list<EmailSender::Person> to {
		EmailSender::Person("qwert", "9876543")
	};
	std::list<EmailSender::Person> cc;

	EmailSender emailSender(from, to, cc);
	EXPECT_THAT(emailSender.createFullMessage("MySubject", "MyMessage"), Eq(expectedMessage));
}

TEST(EmailSenderTest, createFullMessage2) {
	const std::string expectedMessage =
			"From: abcdef <12345678>\r\n"
			"To: qwert1 <1_9876543>, qwert2 <2_9876543>\r\n"
			"Subject: MySubject\r\n"
			"\r\n"
			"MyMessage";

	EmailSender::Person from("abcdef", "12345678");
	std::list<EmailSender::Person> to {
		EmailSender::Person("qwert1", "1_9876543"),
		EmailSender::Person("qwert2", "2_9876543")
	};
	std::list<EmailSender::Person> cc;

	EmailSender emailSender(from, to, cc);
	EXPECT_THAT(emailSender.createFullMessage("MySubject", "MyMessage"), Eq(expectedMessage));
}

TEST(EmailSenderTest, createFullMessage3) {
	const std::string expectedMessage =
			"From: abcdef <12345678>\r\n"
			"To: qwert1 <1_9876543>\r\n"
			"Cc: qwert2 <2_9876543>\r\n"
			"Subject: MySubject\r\n"
			"\r\n"
			"MyMessage";

	EmailSender::Person from("abcdef", "12345678");
	std::list<EmailSender::Person> to {
		EmailSender::Person("qwert1", "1_9876543")
	};
	std::list<EmailSender::Person> cc {
		EmailSender::Person("qwert2", "2_9876543")
	};

	EmailSender emailSender(from, to, cc);
	EXPECT_THAT(emailSender.createFullMessage("MySubject", "MyMessage"), Eq(expectedMessage));
}

