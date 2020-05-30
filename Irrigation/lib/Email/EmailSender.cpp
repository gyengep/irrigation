#include "EmailSender.h"
#include "CurlStringReader.h"
#include "Logger/Logger.h"
#include <curl/curl.h>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace std;


EmailSender::Person::Person(const std::string& name, const std::string& address) :
	name(name),
	address(address)
{
	if (address.empty()) {
		throw std::runtime_error("The email address must not be empty");
	}
}

std::string EmailSender::Person::toString() const {
	std::ostringstream oss;

	if (name.empty()) {
		oss << address;
	} else {
		oss << name << " <" + address + ">";
	}

	return oss.str();
}

std::string EmailSender::Person::toString(const std::list<Person>& persons) {
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

EmailSender::EmailSender(const Person& from, const std::list<Person>& to, const std::list<Person>& cc) :
	url("smtps://smtp.gmail.com:465"),
	username("irrigation.gyengep"),
	password("hFlTL4931c"),
	from(from),
	to(to),
	cc(cc)
{
	if (to.empty()) {
		throw std::runtime_error("The email to list must not be empty");
	}
}

EmailSender::~EmailSender() {
}

void EmailSender::send(const std::string& subject, const std::string& message) {

	LOGGER.trace("curl_easy_init()");
	unique_ptr<CURL, void (*)(CURL*)> curl(curl_easy_init(), curl_easy_cleanup);

	if (curl.get() == nullptr) {
		throw logic_error("NetworkReader::read()  curl == nullptr");
	}

	const std::string fullMessage = createFullMessage(subject, message);
	LOGGER.trace("message: %s", fullMessage.c_str());
	CurlStringReader curlStringReader(fullMessage);

	struct curl_slist *recipients = NULL;
	char errbuf[CURL_ERROR_SIZE] = {0};

	for (const auto& recipient : to) {
		recipients = curl_slist_append(recipients, recipient.address.c_str());
	}

	for (const auto& recipient : cc) {
		recipients = curl_slist_append(recipients, recipient.address.c_str());
	}

	curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER, errbuf);
	curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_PASSWORD, password.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_MAIL_FROM, from.address.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_MAIL_RCPT, recipients);
	curl_easy_setopt(curl.get(), CURLOPT_READFUNCTION, CurlStringReader::readFunction);
	curl_easy_setopt(curl.get(), CURLOPT_READDATA, &curlStringReader);
	curl_easy_setopt(curl.get(), CURLOPT_UPLOAD, 1L);

	LOGGER.trace("curl_easy_perform()");
	const CURLcode curlCode = curl_easy_perform(curl.get());
	LOGGER.trace("curl_easy_perform() result: %d", (int)curlCode);

	LOGGER.trace("curl_slist_free_all()");
	curl_slist_free_all(recipients);

	if (CURLE_OK != curlCode) {
		const char* errorMessage = errbuf;

		if (errorMessage[0] == '\0') {
			errorMessage = curl_easy_strerror(curlCode);
		}

		LOGGER.warning("curl_easy_perform() failed: %s, %s", std::to_string(curlCode).c_str(), errorMessage);
	}

}

std::string EmailSender::createFullMessage(const std::string& subject, const std::string& messageBody) {
	std::ostringstream oss;

	oss << "From: " << from.toString() << "\r\n";

	if (!to.empty()) {
		oss << "To: " << Person::toString(to) << "\r\n";
	}

	if (!cc.empty()) {
		oss << "Cc: " << Person::toString(cc) << "\r\n";
	}

	oss << "Subject: " << subject << "\r\n";
	oss << "\r\n";
	oss << messageBody;

	return oss.str();
}
