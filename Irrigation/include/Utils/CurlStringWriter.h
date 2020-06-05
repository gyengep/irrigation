#pragma once
#include <string>
#include <sstream>


class CurlStringWriter {
	std::string text;

	size_t writeNextPart(char *buffer, size_t itemSize, size_t nmemb);

public:
	CurlStringWriter();
	virtual ~CurlStringWriter();

	const std::string& getText() const;

	static size_t writeFunction(char* buffer, size_t itemSize, size_t nmemb, void* context);
};
