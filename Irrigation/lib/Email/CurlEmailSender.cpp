#include "CurlEmailSender.h"
#include "Utils/CurlStringReader.h"
#include <curl/curl.h>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

string getSendInBlueSmtpKey() {
	// sendinblue_smtp_key is set in makefile
	return sendinblue_smtp_key;
}

std::shared_ptr<EmailSender> EmailSender::create() {
	return std::make_shared<CurlEmailSender>(
		"smtp://smtp-relay.sendinblue.com:587",
		"gyengep@gmail.com",
		getSendInBlueSmtpKey()
	);
}

///////////////////////////////////////////////////////////////////////////////

CurlEmailSender::CurlEmailSender(const std::string& url, const std::string& username, const std::string& password) :
	url(url),
	username(username),
	password(password)
{
}

CurlEmailSender::~CurlEmailSender() {
}

void CurlEmailSender::send(const Email& email) {

	if (email.to.empty()) {
		throw std::runtime_error("The emailTo list must not be empty");
	}

	unique_ptr<CURL, void (*)(CURL*)> curl(curl_easy_init(), curl_easy_cleanup);

	if (curl.get() == nullptr) {
		throw logic_error("NetworkReader::read()  curl == nullptr");
	}

	struct curl_slist *recipients = NULL;

	for (const auto& recipient : email.to) {
		recipients = curl_slist_append(recipients, recipient.address.c_str());
	}

	for (const auto& recipient : email.cc) {
		recipients = curl_slist_append(recipients, recipient.address.c_str());
	}

	CurlStringReader curlStringReader(email.toString());
	char errbuf[CURL_ERROR_SIZE] = {0};

	curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER, errbuf);
	curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_PASSWORD, password.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_MAIL_FROM, email.from.address.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_MAIL_RCPT, recipients);
	curl_easy_setopt(curl.get(), CURLOPT_USE_SSL, CURLUSESSL_ALL);
	curl_easy_setopt(curl.get(), CURLOPT_READFUNCTION, CurlStringReader::readFunction);
	curl_easy_setopt(curl.get(), CURLOPT_READDATA, &curlStringReader);
	curl_easy_setopt(curl.get(), CURLOPT_UPLOAD, 1L);

	const CURLcode curlCode = curl_easy_perform(curl.get());

	curl_slist_free_all(recipients);

	if (CURLE_OK != curlCode) {
		const char* errorMessage = errbuf;

		if (errorMessage[0] == '\0') {
			errorMessage = curl_easy_strerror(curlCode);
		}

		throw runtime_error("curl_easy_perform() failed: " + to_string(curlCode) + ", " + errorMessage);
	}
}
