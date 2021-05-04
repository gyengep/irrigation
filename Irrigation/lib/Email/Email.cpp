#include "Email.h"
#include <sstream>

using namespace std;


Email::Contact::Contact(const std::string& name, const std::string& address) :
	name(name),
	address(address)
{
	if (address.empty()) {
		throw std::runtime_error("The email address must not be empty");
	}
}

bool Email::Contact::operator==(const Email::Contact& other) const {
	return (
		other.name == name &&
		other.address == address
	);
}

std::string Email::Contact::toString() const {
	std::ostringstream oss;

	if (name.empty()) {
		oss << address;
	} else {
		oss << name << " <" + address + ">";
	}

	return oss.str();
}

std::string Email::Contact::toString(const std::list<Contact>& persons) {
	if (persons.empty()) {
		throw std::runtime_error("The email contact list must not be empty");
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

Email::Email(
	const Contact& from,
	const std::list<Contact>& to,
	const std::list<Contact>& cc,
	const std::string& subject,
	const std::string& text,
	const std::time_t& date
) :
	from(from),
	to(to),
	cc(cc),
	subject(subject),
	text(text),
	date(date)
{
}

bool Email::operator==(const Email& other) const {
	return (
		other.from == from &&
		other.to == to &&
		other.cc == cc &&
		other.subject == subject &&
		other.text == text &&
		other.date == date
	);
}

std::string Email::toString() const {
	std::ostringstream oss;

	oss << "From: " << from.toString() << "\r\n";

	if (!to.empty()) {
		oss << "To: " << Contact::toString(to) << "\r\n";
	}

	if (!cc.empty()) {
		oss << "Cc: " << Contact::toString(cc) << "\r\n";
	}

	oss << "Subject: " << subject << "\r\n";
	oss << "Date: " << dateToString(date) << "\r\n";
	oss << "\r\n";
	oss << text;

	return oss.str();
}

std::string Email::dateToString(const std::time_t& rawTime) {
	const int BufferSize = 100;
	char buffer[BufferSize];

	tm timeinfo;
	localtime_r(&rawTime, &timeinfo);

	strftime(buffer, BufferSize, "%a, %d %b %G %H:%M:%S %z", &timeinfo);
	return buffer;
}
