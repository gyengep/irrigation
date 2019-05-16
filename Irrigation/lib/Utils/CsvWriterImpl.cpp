#include "CsvWriterImpl.h"
#include <stdexcept>

using namespace std;


CsvWriterImpl::CsvWriterImpl(const std::shared_ptr<std::ofstream>& ofs, char separator) :
	CsvWriter(separator),
	ofs(ofs)
{
}

CsvWriterImpl::~CsvWriterImpl() {
}

void CsvWriterImpl::append(const vector<string>& values) {
	for (auto it = values.begin(); values.end() != it; ++it) {
		if (values.begin() != it) {
			*ofs << separator;
		}

		*ofs << *it;
	}

	*ofs << endl;
}
