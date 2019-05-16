#include "TemperatureFile.h"
#include "Utils/CsvWriterImpl.h"

using namespace std;


TemperatureFile::TemperatureFile(const string& filePath) :
	fileName()
{
	shared_ptr<ofstream> ofs = make_shared<ofstream>();
	ofs->open(fileName);

	CsvWriterImpl(ofs, ',').append(vector<string>{"Date", "Temperature"});
}

TemperatureFile::~TemperatureFile() {
}

