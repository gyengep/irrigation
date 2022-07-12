#include <gmock/gmock.h>
#include "Email/EmailSender.h"

using namespace std;
using namespace testing;


TEST(EmailSenderTest, send) {

	EmailSender::create()->send(
		Email(
			Email::Contact("Irrigation System", "irrigation.gyengep@gmail.com"),
			std::list<Email::Contact>{ Email::Contact("Gyenge Peter", "gyengep@gmail.com") },
			std::list<Email::Contact>{},
			"Test Subject",
			"Test Message",
			DateTime::now()
		)
	);
}
