#include "CurlStringReader.h"
#include "Exceptions/IllegalArgumentException.h"
#include <cstring>


CurlStringReader::CurlStringReader(const std::string& text) :
	text(text),
	pos(0)
{
}

CurlStringReader::~CurlStringReader() {
}

size_t CurlStringReader::readNextPart(void *ptr, size_t size, size_t nmemb) {

	if (nullptr == ptr) {
		throw IllegalArgumentException("CurlStringReader::readNextPart() nullptr == ptr");
	}

	if ((size == 0) || (nmemb == 0)) {
		return 0;
	}

	const size_t remainingSize = text.size() - pos;
	size_t readedBytes = size * nmemb;

	if (readedBytes > remainingSize) {
		nmemb = remainingSize / size;
		readedBytes = size * nmemb;
	}

	if (readedBytes > 0) {
		std::memcpy(ptr, text.data() + pos, readedBytes);
		pos += readedBytes;
	}

	return nmemb;
}

size_t CurlStringReader::readFunction(void *ptr, size_t size, size_t nmemb, void *userp) {
	if (nullptr == userp) {
		throw IllegalArgumentException("CurlStringReader::readFunction() nullptr == userp");
	}

	return static_cast<CurlStringReader*>(userp)->readNextPart(ptr, size, nmemb);
}
