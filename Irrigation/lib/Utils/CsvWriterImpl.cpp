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

CsvWriterFactoryImpl::CsvWriterFactoryImpl(const std::string& fileName) :
	fileName(fileName)
{
}

CsvWriterFactoryImpl::~CsvWriterFactoryImpl() {
}

shared_ptr<CsvWriter> CsvWriterFactoryImpl::create() {
	auto ofs = std::make_shared<std::ofstream>(fileName, ofstream::out | ofstream::app);

	if (ofs->fail()) {
		throw IOException(errno);
	}

	return make_shared<CsvWriterImpl>(ofs);
}
