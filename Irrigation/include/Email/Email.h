#pragma once
#include <ctime>
#include <list>
#include <string>
#include "Utils/DateTime.h"


struct Email {

	struct Contact {
		std::string name;
		std::string address;

		Contact(const std::string& name, const std::string& address);

		bool operator==(const Contact& other) const;

		std::string toString() const;
		static std::string toString(const std::list<Contact>& contacts);
	};

	Contact from;
	std::list<Contact> to;
	std::list<Contact> cc;
	std::string subject;
	std::string text;
	DateTime dateTime;

	Email(
		const Contact& from,
		const std::list<Contact>& to,
		const std::list<Contact>& cc,
		const std::string& subject,
		const std::string& text,
		const DateTime& dateTime
	);

	bool operator==(const Email& other) const;

	std::string toString() const;
};

