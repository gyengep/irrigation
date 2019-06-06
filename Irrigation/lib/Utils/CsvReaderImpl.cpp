#include "CsvReaderImpl.h"
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

CsvReaderImplFactory::~CsvReaderImplFactory() {
}

shared_ptr<CsvReader> CsvReaderImplFactory::create(const shared_ptr<istream>& input) {
	return make_shared<CsvReaderImpl>(input);
}
