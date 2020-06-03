#include "EmailSender.h"
#include <sstream>

using namespace std;


Contact::Contact(const std::string& name, const std::string& address) :
	name(name),
	address(address)
{
	if (address.empty()) {
		throw std::runtime_error("The email address must not be empty");
	}
}

std::string Contact::toString() const {
	std::ostringstream oss;

	if (name.empty()) {
		oss << address;
	} else {
		oss << name << " <" + address + ">";
	}

	return oss.str();
}

std::string Contact::toString(const std::list<Contact>& persons) {
	if (persons.empty()) {
		throw std::runtime_error("The email list must not be empty");
	}

	std::ostringstream oss;

	for (auto it = persons.begin(); it != persons.end(); ++it) {
		if (it != persons.begin())  {
			oss << ", ";
		}

		oss << it->toString();
	}

	return oss.str();
}

///////////////////////////////////////////////////////////////////////////////

std::string Message::toString() const {
	std::ostringstream oss;

	oss << "From: " << from.toString() << "\r\n";

	if (!to.empty()) {
		oss << "To: " << Contact::toString(to) << "\r\n";
	}

	if (!cc.empty()) {
		oss << "Cc: " << Contact::toString(cc) << "\r\n";
	}

	oss << "Subject: " << subject << "\r\n";
	oss << "\r\n";
	oss << text;

	return oss.str();
}
