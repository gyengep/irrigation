#include <gmock/gmock.h>
#include "Email/Email.h"

using namespace std;
using namespace testing;


TEST(EmailTest, contact) {
	const std::string name("abcd");
	const std::string address("123456");

	Email::Contact contact(name, address);

	EXPECT_THAT(contact.name, Eq(name));
	EXPECT_THAT(contact.address, Eq(address));
}

TEST(EmailTest, contactWithoutName) {
	const std::string name;
	const std::string address("123456");

	Email::Contact contact(name, address);

	EXPECT_THAT(contact.name, Eq(name));
	EXPECT_THAT(contact.address, Eq(address));
}

TEST(EmailTest, contactWithoutAddress) {
	const std::string name("abcd");
	const std::string address;

	EXPECT_THROW(Email::Contact(name, address), std::runtime_error);
}

TEST(EmailTest, contactToString) {
	EXPECT_THAT(Email::Contact("abcd", "123456").toString(), Eq("abcd <123456>"));
	EXPECT_THAT(Email::Contact("", "123456").toString(), Eq("123456"));
}

TEST(EmailTest, contactListToString) {
	std::list<Email::Contact> contacts {
		Email::Contact("abcd", "123456"),
		Email::Contact("xyz", "987")
	};

	EXPECT_THAT(Email::Contact::toString(contacts), Eq("abcd <123456>, xyz <987>"));
}

TEST(EmailTest, emptyPersonListToString) {
	EXPECT_THROW(Email::Contact::toString(std::list<Email::Contact>()), std::runtime_error);
}

///////////////////////////////////////////////////////////////////////////////

TEST(EmailTest, toString1) {

	const std::string expectedMessage =
			"From: abcdef <12345678>\r\n"
			"To: qwert <9876543>\r\n"
			"Subject: MySubject\r\n"
			"Date: Mon, 25 Nov 2019 08:08:09 +0100\r\n"
			"\r\n"
			"MyMessage";

	Email email(
			Email::Contact("abcdef", "12345678"),
			std::list<Email::Contact>{Email::Contact("qwert", "9876543")},
			std::list<Email::Contact>(),
			"MySubject",
			"MyMessage",
			LocalDateTime::create(2019, 11, 25, 8, 8, 9)
		);

	EXPECT_THAT(email.toString(), Eq(expectedMessage));
}

TEST(EmailTest, toString2) {
	const std::string expectedMessage =
			"From: abcdef <12345678>\r\n"
			"To: qwert1 <1_9876543>, qwert2 <2_9876543>\r\n"
			"Subject: MySubject\r\n"
			"Date: Fri, 17 Jan 2020 17:37:48 +0100\r\n"
			"\r\n"
			"MyMessage";

	Email email(
			Email::Contact("abcdef", "12345678"),
			std::list<Email::Contact>{Email::Contact("qwert1", "1_9876543"), Email::Contact("qwert2", "2_9876543")},
			std::list<Email::Contact>(),
			"MySubject",
			"MyMessage",
			LocalDateTime::create(2020, 1, 17, 17, 37, 48)
		);

	EXPECT_THAT(email.toString(), Eq(expectedMessage));
}

TEST(EmailTest, toString3) {
	const std::string expectedMessage =
			"From: abcdef <12345678>\r\n"
			"To: qwert1 <1_9876543>\r\n"
			"Cc: qwert2 <2_9876543>\r\n"
			"Subject: MySubject\r\n"
			"Date: Wed, 26 Aug 2020 22:34:53 +0200\r\n"
			"\r\n"
			"MyMessage";

	Email email(
			Email::Contact("abcdef", "12345678"),
			std::list<Email::Contact>{Email::Contact("qwert1", "1_9876543")},
			std::list<Email::Contact>{Email::Contact("qwert2", "2_9876543")},
			"MySubject",
			"MyMessage",
			LocalDateTime::create(2020, 8, 26, 22, 34, 53)
		);

	EXPECT_THAT(email.toString(), Eq(expectedMessage));
}
