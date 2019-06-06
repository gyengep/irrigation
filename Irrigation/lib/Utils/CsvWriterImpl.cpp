#include "CsvWriterImpl.h"
#include <stdexcept>

using namespace std;


CsvWriterImpl::CsvWriterImpl(const std::shared_ptr<std::ostream>& output) :
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

CsvWriterImplFactory::~CsvWriterImplFactory() {
}

std::shared_ptr<CsvWriter> CsvWriterImplFactory::create(const std::shared_ptr<std::ostream>& output) {
	return make_shared<CsvWriterImpl>(output);
}
