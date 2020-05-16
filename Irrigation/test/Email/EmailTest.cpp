#include <gmock/gmock.h>
#include <fstream>
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"
#include "Email/Email.h"

using namespace std;


TEST(EmailTest, test) {
	LOGGER.setLevel(LogLevel::TRACE);
	LOGGER.setOutputStream(std::cout);

	LOGGER.trace("TEST(EmailTest, test) __BEGIN__");
	EMAIL.send(EmailTopic::WATERING, "Message Body");
	LOGGER.trace("TEST(EmailTest, test) __END__");
}
