#include "CsvWriterImpl.h"
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

const shared_ptr<ostream>& CsvWriterImpl::stream() const {
	return output;
}

///////////////////////////////////////////////////////////////////////////////

CsvWriterImplFactory::~CsvWriterImplFactory() {
}

shared_ptr<CsvWriter> CsvWriterImplFactory::create(const shared_ptr<ostream>& output) {
	return make_shared<CsvWriterImpl>(output);
}
