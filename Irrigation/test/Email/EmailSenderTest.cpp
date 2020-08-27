#include <gmock/gmock.h>
#include "Email/EmailSender.h"

using namespace std;
using namespace testing;


TEST(ContactTest, contact) {
	const std::string name("abcd");
	const std::string address("123456");

	Contact contact(name, address);

	EXPECT_THAT(contact.name, Eq(name));
	EXPECT_THAT(contact.address, Eq(address));
}

TEST(ContactTest, contactWithoutName) {
	const std::string name;
	const std::string address("123456");

	Contact contact(name, address);

	EXPECT_THAT(contact.name, Eq(name));
	EXPECT_THAT(contact.address, Eq(address));
}

TEST(ContactTest, contactWithoutAddress) {
	const std::string name("abcd");
	const std::string address;

	EXPECT_THROW(Contact(name, address), std::runtime_error);
}

TEST(ContactTest, contactToString) {
	EXPECT_THAT(Contact("abcd", "123456").toString(), Eq("abcd <123456>"));
	EXPECT_THAT(Contact("", "123456").toString(), Eq("123456"));
}

TEST(ContactTest, contactListToString) {
	std::list<Contact> contacts {
		Contact("abcd", "123456"),
		Contact("xyz", "987")
	};

	EXPECT_THAT(Contact::toString(contacts), Eq("abcd <123456>, xyz <987>"));
}

TEST(ContactTest, emptyPersonListToString) {
	EXPECT_THROW(Contact::toString(std::list<Contact>()), std::runtime_error);
}

///////////////////////////////////////////////////////////////////////////////

TEST(MessageTest, toString1) {

	const std::string expectedMessage =
			"From: abcdef <12345678>\r\n"
			"To: qwert <9876543>\r\n"
			"Subject: MySubject\r\n"
			"Date: Mon, 25 Nov 2019 08:08:09 +0100\r\n"
			"\r\n"
			"MyMessage";

	::Message message;

	message.from = Contact("abcdef", "12345678");
	message.to.push_back(Contact("qwert", "9876543"));
	message.subject = "MySubject";
	message.text = "MyMessage";
	message.date = 1574665689;

	EXPECT_THAT(message.toString(), Eq(expectedMessage));
}

TEST(MessageTest, toString2) {
	const std::string expectedMessage =
			"From: abcdef <12345678>\r\n"
			"To: qwert1 <1_9876543>, qwert2 <2_9876543>\r\n"
			"Subject: MySubject\r\n"
			"Date: Fri, 17 Jan 2020 17:37:48 +0100\r\n"
			"\r\n"
			"MyMessage";

	::Message message;

	message.from = Contact("abcdef", "12345678");
	message.to.push_back(Contact("qwert1", "1_9876543"));
	message.to.push_back(Contact("qwert2", "2_9876543"));
	message.subject = "MySubject";
	message.text = "MyMessage";
	message.date = 1579279068;

	EXPECT_THAT(message.toString(), Eq(expectedMessage));
}

TEST(MessageTest, toString3) {
	const std::string expectedMessage =
			"From: abcdef <12345678>\r\n"
			"To: qwert1 <1_9876543>\r\n"
			"Cc: qwert2 <2_9876543>\r\n"
			"Subject: MySubject\r\n"
			"Date: Wed, 26 Aug 2020 22:34:53 +0200\r\n"
			"\r\n"
			"MyMessage";

	::Message message;

	message.from = Contact("abcdef", "12345678");
	message.to.push_back(Contact("qwert1", "1_9876543"));
	message.cc.push_back(Contact("qwert2", "2_9876543"));
	message.subject = "MySubject";
	message.text = "MyMessage";
	message.date = 1598474093;

	EXPECT_THAT(message.toString(), Eq(expectedMessage));
}

TEST(MessageTest, dateToString1) {
	//1598474093:  2020-08-26T20:34:53+00:00
	EXPECT_THAT(::Message::dateToString(1598474093), Eq("Wed, 26 Aug 2020 22:34:53 +0200"));
}

TEST(MessageTest, dateToString2) {
	//1579279068:  2020-01-17T16:37:48+00:00
	EXPECT_THAT(::Message::dateToString(1579279068), Eq("Fri, 17 Jan 2020 17:37:48 +0100"));
}

TEST(MessageTest, dateToString3) {
	//1574665689:  2019-11-25T07:08:09+00:00
	EXPECT_THAT(::Message::dateToString(1574665689), Eq("Mon, 25 Nov 2019 08:08:09 +0100"));
}



