#include "CsvReaderImpl.h"
#include "Exceptions/IOException.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;


CsvReaderImpl::CsvReaderImpl(const shared_ptr<istream>& input) :
	input(input)
{
}

CsvReaderImpl::~CsvReaderImpl() {
}

unique_ptr<vector<string>> CsvReaderImpl::read() {
	string line;

	while (getline(*input, line)) {
		if (!line.empty()) {
			istringstream iss(line);
			string part;
			unique_ptr<vector<string>> result(new vector<string>());

			while (getline(iss, part, separator)) {
				result->push_back(part);
			}

			return result;
		}
	}

	return unique_ptr<vector<string>>();
}

///////////////////////////////////////////////////////////////////////////////

CsvReaderFactoryImpl::CsvReaderFactoryImpl(const std::string& fileName) :
	fileName(fileName)
{
}

CsvReaderFactoryImpl::~CsvReaderFactoryImpl() {
}

shared_ptr<CsvReader> CsvReaderFactoryImpl::create() {
	auto ifs = std::make_shared<std::ifstream>(fileName, std::ofstream::in);

	if (ifs->fail()) {
		throw IOException(errno);
	}

	return make_shared<CsvReaderImpl>(ifs);
}
