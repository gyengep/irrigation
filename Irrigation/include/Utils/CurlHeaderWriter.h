#pragma once
#include <list>
#include <string>


class CurlHeaderWriter {
	std::list<std::string> headers;

	size_t writeNextPart(char* buffer, size_t itemSize, size_t itemCount);

public:
	CurlHeaderWriter();
	virtual ~CurlHeaderWriter();

	const std::list<std::string>& getHeaders() const;

	static size_t headerFunction(char* buffer, size_t itemSize, size_t itemCount, void* context);
};
