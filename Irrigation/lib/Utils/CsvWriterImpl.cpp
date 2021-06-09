#include "CsvWriterImpl.h"
#include "Exceptions/IOException.h"
#include <fstream>
#include <stdexcept>

using namespace std;


CsvWriterImpl::CsvWriterImpl(const shared_ptr<ostream>& output) :
	output(output)
{
}

CsvWriterImpl::~CsvWriterImpl() {
}

void CsvWriterImpl::append(const vector<string>& values) {
	for (auto it = values.begin(); values.end() != it; ++it) {
		if (values.begin() != it) {
			*output << separator;
		}

		*output << *it;
	}

	*output << endl;
}

///////////////////////////////////////////////////////////////////////////////

CsvWriterFactoryImpl::CsvWriterFactoryImpl(const std::string& fileName, bool append) :
	fileName(fileName),
	append(append)
{
}

CsvWriterFactoryImpl::~CsvWriterFactoryImpl() {
}

shared_ptr<CsvWriter> CsvWriterFactoryImpl::create() {
	std::ios_base::openmode fileOpenMode;

	if (append) {
		fileOpenMode = ofstream::out | ofstream::app;
	} else {
		fileOpenMode = ofstream::out;

	}

	auto ofs = std::make_shared<std::ofstream>(fileName, fileOpenMode);

	if (ofs->fail()) {
		throw IOException(errno);
	}

	return make_shared<CsvWriterImpl>(ofs);
}
