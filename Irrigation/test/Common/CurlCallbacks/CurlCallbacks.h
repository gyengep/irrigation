#pragma once
#include <algorithm>
#include <list>
#include <map>
#include <sstream>
#include <string>

typedef std::map<std::string, std::string> KeyValue;

struct WriteCallbackData {
	std::string text;
};

struct ReadCallbackData {
	std::string text;
	size_t position;

	ReadCallbackData(const std::string& text) : text(text), position(0) {}
};

struct HeaderCallbackData {
	std::list<std::string> headers;
};


std::string createUrl(uint16_t port, const std::string& path, const KeyValue& parameters);
size_t writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);
size_t readCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);
size_t headerCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);


std::string createUrl(uint16_t port, const std::string& path, const KeyValue& parameters) {
	if ('/' != path[0]) {
		throw std::logic_error("The path has to start with '/'");
	}

	std::ostringstream o;

	o << "http://localhost:" << port;
	o << path;

	if (!parameters.empty()) {
		o << "?";

		for (auto it = parameters.begin(); it != parameters.end(); ++it) {
			if (parameters.begin() != it) {
				o << "&";
			}

			o << it->first << "=" << it->second;
		}
	}

	return o.str();
}

size_t headerCallback(char* buffer, size_t size, size_t nmemb, void* ctxt) {
	HeaderCallbackData* headerCallbackData = static_cast<HeaderCallbackData*>(ctxt);

    if (!headerCallbackData) {
    	throw std::logic_error("headerCallback()  nullptr == headerCallbackData");
    }

    const size_t length = size * nmemb;
    headerCallbackData->headers.push_back(std::string(buffer, length));
	return length;
}

size_t writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt) {
	WriteCallbackData* writeCallbackData = static_cast<WriteCallbackData*>(ctxt);

    if (!writeCallbackData) {
    	throw std::logic_error("writeCallback()  nullptr == writeCallbackData");
    }

    const size_t length = size * nmemb;
    writeCallbackData->text.append(&buffer[0], &buffer[length]);
   	return length;
}

size_t readCallback(char* buffer, size_t size, size_t nmemb, void* ctxt) {
	ReadCallbackData* readCallbackData = static_cast<ReadCallbackData*>(ctxt);

	const size_t maxDataSize = size * nmemb;
	const size_t remainingDataSize = readCallbackData->text.length() - readCallbackData->position;
	const size_t length = std::min(maxDataSize, remainingDataSize);

	std::copy_n(&readCallbackData->text[readCallbackData->position], length, buffer);
    readCallbackData->position += length;
    return length;
}


