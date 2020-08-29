#pragma once
#include <ctime>
#include <list>
#include <string>


struct Contact {
	std::string name;
	std::string address;

	Contact() = default;
	Contact(const std::string& name, const std::string& address);

	std::string toString() const;
	static std::string toString(const std::list<Contact>& contacts);
};

///////////////////////////////////////////////////////////////////////////////

struct Message {
	Contact from;
	std::list<Contact> to;
	std::list<Contact> cc;
	std::string subject;
	std::string text;
	std::time_t date;

	Message() = default;

	std::string toString() const;
	static std::string dateToString(const std::time_t& rawTime);
};

///////////////////////////////////////////////////////////////////////////////

class EmailSender {
public:
	virtual ~EmailSender() = default;
	virtual void send(const Message& message) = 0;
};
