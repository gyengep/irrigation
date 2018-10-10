#include "FileReaderWriterImpl.h"
#include <fstream>
#include "Exceptions/Exceptions.h"

using namespace std;


string FileReaderImpl::read(const string& fileName) const {
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

void FileWriterImpl::write(const string& fileName, const string& text) const {
	throw logic_error("Function not implemented FileWriterImpl::write()");

	//	ofstream ofs;
	//
	//	ofs.open(fileName, ofstream::out | ofstream::trunc);
	//	if (ofs.fail()) {
	//		throw IOException(errno);
	//	}
	//
	//	ofs << text;
	//	if (ofs.fail()) {
	//		throw IOException(errno);
	//	}
	//
	//	ofs.close();
	//	if (ofs.fail()) {
	//		throw IOException(errno);
	//	}
}

