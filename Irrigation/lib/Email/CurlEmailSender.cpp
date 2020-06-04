#include "CurlEmailSender.h"
#include "Logger/Logger.h"
#include "Utils/CurlStringReader.h"
#include <curl/curl.h>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace std;


CurlEmailSender::CurlEmailSender() :
	url("smtps://smtp.gmail.com:465"),
	username("irrigation.gyengep"),
	password("hFlTL4931c")
{
}

CurlEmailSender::~CurlEmailSender() {
}

void CurlEmailSender::send(const Message& message) {

	if (message.to.empty()) {
		throw std::runtime_error("The email to list must not be empty");
	}

	unique_ptr<CURL, void (*)(CURL*)> curl(curl_easy_init(), curl_easy_cleanup);

	if (curl.get() == nullptr) {
		throw logic_error("NetworkReader::read()  curl == nullptr");
	}

	CurlStringReader curlStringReader(message.toString());

	struct curl_slist *recipients = NULL;
	char errbuf[CURL_ERROR_SIZE] = {0};

	for (const auto& recipient : message.to) {
		recipients = curl_slist_append(recipients, recipient.address.c_str());
	}

	for (const auto& recipient : message.cc) {
		recipients = curl_slist_append(recipients, recipient.address.c_str());
	}

	curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER, errbuf);
	curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_PASSWORD, password.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_MAIL_FROM, message.from.address.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_MAIL_RCPT, recipients);
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

		LOGGER.warning("curl_easy_perform() failed: %s, %s", std::to_string(curlCode).c_str(), errorMessage);
	}
}
