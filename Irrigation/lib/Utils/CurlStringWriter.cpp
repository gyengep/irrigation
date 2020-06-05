#include "CurlStringWriter.h"
#include "Exceptions/IllegalArgumentException.h"

using namespace std;


CurlStringWriter::CurlStringWriter() {
}

CurlStringWriter::~CurlStringWriter() {
}

size_t CurlStringWriter::writeNextPart(char *buffer, size_t itemSize, size_t itemCount) {

	if (nullptr == buffer) {
		throw IllegalArgumentException("CurlStringWriter::readNextPart() nullptr == ptr");
	}

	if ((itemSize == 0) || (itemCount == 0)) {
		return 0;
	}

	const size_t bytes = itemSize * itemCount;
	text.append(buffer, bytes);
	return itemCount;
}

size_t CurlStringWriter::writeFunction(char* buffer, size_t itemSize, size_t itemCount, void* context) {
	if (nullptr == context) {
		throw IllegalArgumentException("CurlStringWriter::writeFunction() nullptr == context");
	}

	return static_cast<CurlStringWriter*>(context)->writeNextPart(buffer, itemSize, itemCount);
}

const std::string& CurlStringWriter::getText() const {
	return text;
}
