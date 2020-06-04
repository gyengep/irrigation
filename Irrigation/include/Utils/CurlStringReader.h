#pragma once
#include <string>


class CurlStringReader {
	const std::string text;
	size_t pos;

	size_t readNextPart(void *ptr, size_t size, size_t nmemb);

public:
	CurlStringReader(const std::string& text);
	virtual ~CurlStringReader();

	static size_t readFunction(void *ptr, size_t size, size_t nmemb, void *userp);
};
