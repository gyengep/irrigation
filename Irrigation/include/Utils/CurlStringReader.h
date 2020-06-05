#pragma once
#include <string>


class CurlStringReader {
	const std::string text;
	size_t pos;

	size_t readNextPart(void *buffer, size_t itemSize, size_t itemCount);

public:
	CurlStringReader(const std::string& text);
	virtual ~CurlStringReader();

	static size_t readFunction(void *buffer, size_t itemSize, size_t itemCount, void *context);
};
