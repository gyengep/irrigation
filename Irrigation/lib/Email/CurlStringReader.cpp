#include "CurlStringReader.h"
#include <cstring>


CurlStringReader::CurlStringReader(const std::string& text) :
	text(text),
	pos(0)
{
}

CurlStringReader::~CurlStringReader() {
}

size_t CurlStringReader::readNextPart(void *ptr, size_t size, size_t nmemb) {

	if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
		return 0;
	}

	const size_t remainingSize = text.size() - pos;
	const size_t uploadSize = std::min(size, remainingSize);

	if (0 < uploadSize) {
		std::memcpy(ptr, text.data() + pos, uploadSize);
		pos += uploadSize;
	}

	return uploadSize;
}

size_t CurlStringReader::readFunction(void *ptr, size_t size, size_t nmemb, void *userp) {
	return static_cast<CurlStringReader*>(userp)->readNextPart(ptr, size, nmemb);
}
