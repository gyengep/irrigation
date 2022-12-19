#include "Request.h"
#include "Response.h"
#include "Utils/CurlHeaderWriter.h"
#include "Utils/CurlStringReader.h"
#include "Utils/CurlStringWriter.h"
#include <curl/curl.h>

using namespace AAA;


Request::Request(const std::string& method, const std::string&  url) :
	method(method),
	url(url)
{
}

Request& Request::setBody(const std::string& body) {
	this->body = body;
	return *this;
}

Request& Request::setContentType(const std::string& contentType) {
	addHeader("Content-Type: " + contentType);
	return *this;
}

Request& Request::addHeader(const std::string& header) {
	if (false == this->header.empty()) {
		this->header.append("\r\n");
	}

	this->header.append(header);
	return *this;
}

Response Request::execute() {
	CURL *curl = curl_easy_init();
	curl_slist *curlHeader = NULL;

	if (curl == nullptr) {
		throw std::logic_error("RestViewTest::executeRequest()  curl == nullptr");
	}

    CurlStringWriter stringWriter;
    CurlHeaderWriter headerWriter;
	CurlStringReader stringReader(body);
	char errbuf[CURL_ERROR_SIZE] = {0};

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlStringWriter::writeFunction);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stringWriter);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, CurlHeaderWriter::headerFunction);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headerWriter);

	if (!header.empty()) {
		curlHeader = curl_slist_append(curlHeader, header.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curlHeader);
	}

	if (!body.empty()) {
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, CurlStringReader::readFunction);
		curl_easy_setopt(curl, CURLOPT_READDATA, &stringReader);
	}

	const CURLcode curlCode = curl_easy_perform(curl);
	if (CURLE_OK != curlCode) {
		const char* errorMessage = errbuf;

		if (errorMessage[0] == '\0') {
			errorMessage = curl_easy_strerror(curlCode);
		}

		throw std::runtime_error("curl_easy_perform() failed: " + std::to_string(curlCode) + ", " + errorMessage);
	}

	long responseCode;

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
	curl_easy_cleanup(curl);
	curl_slist_free_all(curlHeader);

    return Response(responseCode).
    		addHeaders(headerWriter.getHeaders()).
			setBody(stringWriter.getText());
}
