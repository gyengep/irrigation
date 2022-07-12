#include <gmock/gmock.h>
#include "Email/CurlEmailSender.h"

using namespace std;
using namespace testing;


TEST(CurlEmailSenderTest, send) {

	CurlEmailSender curlEmailSender(
		EmailSender::Properties(
			"smtp://smtp-relay.sendinblue.com:587",
			"gyengep@gmail.com",
			"xsmtpsib-e4af58c44223465ac88102a1c3368208faabdc46620f4cbabfcf6eeb229be03d-wMPC3bL2QxWyFkXY"
		)
	);

	curlEmailSender.send(
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
