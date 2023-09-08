#include "FileReaderImpl.h"
#include "Exceptions/Exceptions.h"
#include <fstream>


FileReaderImpl::FileReaderImpl(const std::string& fileName) :
	fileName(fileName)
{
}

std::string FileReaderImpl::read() const {
	std::ifstream ifs(fileName);

	if (ifs.fail()) {
		if (ENOENT == errno) {
			throw FileNotFoundException();
		} else {
			throw IOException(errno);
		}
	}

	std::string buffer((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	if (ifs.fail()) {
		throw IOException(errno);
	}

	ifs.close();

	if (ifs.fail()) {
		throw IOException(errno);
	}

	return buffer;
}
