#pragma once
#include <list>
#include <string>


class EmailSender {
public:
	struct Person {
		std::string name;
		std::string address;

		Person(const std::string& name, const std::string& address);

		std::string toString() const;
		static std::string toString(const std::list<Person>& persons);
	};

private:
	const std::string url;
	const std::string username;
	const std::string password;

	const Person from;
	const std::list<Person> to;
	const std::list<Person> cc;

public:
	EmailSender(const Person& from, const std::list<Person>& to, const std::list<Person>& cc);
	virtual ~EmailSender();

	void send(const std::string& subject, const std::string& message);
	std::string createFullMessage(const std::string& subject, const std::string& messageBody);
};
