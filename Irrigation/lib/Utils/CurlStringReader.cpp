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

size_t CurlStringReader::readNextPart(void *buffer, size_t itemSize, size_t itemCount) {

	if (nullptr == buffer) {
		throw IllegalArgumentException("CurlStringReader::readNextPart() nullptr == buffer");
	}

	if ((itemSize == 0) || (itemCount == 0)) {
		return 0;
	}

	const size_t remainingSize = text.size() - pos;
	size_t bytes = itemSize * itemCount;

	if (bytes > remainingSize) {
		itemCount = remainingSize / itemSize;
		bytes = itemSize * itemCount;
	}

	if (bytes > 0) {
		std::memcpy(buffer, text.data() + pos, bytes);
		pos += bytes;
	}

	return itemCount;
}

size_t CurlStringReader::readFunction(void *buffer, size_t itemSize, size_t itemCount, void *context) {
	if (nullptr == context) {
		throw IllegalArgumentException("CurlStringReader::readFunction() nullptr == context");
	}

	return static_cast<CurlStringReader*>(context)->readNextPart(buffer, itemSize, itemCount);
}
