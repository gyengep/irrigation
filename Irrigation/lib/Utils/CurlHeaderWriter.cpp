#include "CurlHeaderWriter.h"
#include "Exceptions/IllegalArgumentException.h"

using namespace std;


CurlHeaderWriter::CurlHeaderWriter() {
}

CurlHeaderWriter::~CurlHeaderWriter() {
}

size_t CurlHeaderWriter::writeNextPart(char *buffer, size_t itemSize, size_t itemCount) {

	if (nullptr == buffer) {
		throw IllegalArgumentException("CurlHeaderWriter::readNextPart() nullptr == ptr");
	}

	if ((itemSize == 0) || (itemCount == 0)) {
		return 0;
	}

	const size_t bytes = itemSize * itemCount;
    headers.push_back(std::string(buffer, bytes));
	return itemCount;
}

size_t CurlHeaderWriter::headerFunction(char* buffer, size_t itemSize, size_t itemCount, void* context) {
	if (nullptr == context) {
		throw IllegalArgumentException("CurlHeaderWriter::writeFunction() nullptr == context");
	}

	return static_cast<CurlHeaderWriter*>(context)->writeNextPart(buffer, itemSize, itemCount);
}

const std::list<std::string>& CurlHeaderWriter::getHeaders() const {
	return headers;
}
