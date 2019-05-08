#include "FileReaderWriterImpl.h"
#include <fstream>
#include "Exceptions/Exceptions.h"

using namespace std;


FileReaderImpl::FileReaderImpl(const std::string& fileName) : fileName(fileName) {
}

string FileReaderImpl::read() const {
	ifstream ifs(fileName);

	if (ifs.fail()) {
		if (ENOENT == errno) {
			throw FileNotFoundException();
		} else {
			throw IOException(errno);
		}
	}

	string buffer((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

	if (ifs.fail()) {
		throw IOException(errno);
	}

	ifs.close();

	if (ifs.fail()) {
		throw IOException(errno);
	}

	return buffer;
}

FileWriterImpl::FileWriterImpl(const std::string& fileName) : fileName(fileName) {
}

void FileWriterImpl::write(const string& text) const {
	ofstream ofs;

	ofs.open(fileName, ofstream::out | ofstream::trunc);
	if (ofs.fail()) {
		throw IOException(errno);
	}

	ofs << text;
	if (ofs.fail()) {
		throw IOException(errno);
	}

	ofs.close();
	if (ofs.fail()) {
		throw IOException(errno);
	}
}

