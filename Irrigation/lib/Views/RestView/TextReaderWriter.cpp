#include "TextReaderWriter.h"
#include "DTO/PrintToStream.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Model/IrrigationDocument.h"
#include <sstream>

using namespace std;


string TextErrorWriter::to(const string& errorMessage, unsigned errorCode) {
	return errorMessage;
}

unique_ptr<ErrorWriter> TextErrorWriterFactory::create() const {
	return unique_ptr<ErrorWriter>(new TextErrorWriter());
}

///////////////////////////////////////////////////////////////////////////////

string TextDtoWriter::save(const DocumentDTO& document) {
	ostringstream oss;
	oss << document;
	return oss.str();
}

string TextDtoWriter::save(const ProgramDTO& program) {
	ostringstream oss;
	oss << program;
	return oss.str();
}

string TextDtoWriter::save(const RunTimeDTO& runTime) {
	ostringstream oss;
	oss << runTime;
	return oss.str();
}

string TextDtoWriter::save(const StartTimeDTO& startTime) {
	ostringstream oss;
	oss << startTime;
	return oss.str();
}

string TextDtoWriter::save(const PeriodicSchedulerDTO& scheduler) {
	ostringstream oss;
	oss << scheduler;
	return oss.str();
}

string TextDtoWriter::save(const WeeklySchedulerDTO& scheduler) {
	ostringstream oss;
	oss << scheduler;
	return oss.str();
}

string TextDtoWriter::save(const list<ProgramDTO>& programs) {
	ostringstream oss;
	oss << programs;
	return oss.str();
}

string TextDtoWriter::save(const list<RunTimeDTO>& runTimes) {
	ostringstream oss;
	oss << runTimes;
	return oss.str();
}

string TextDtoWriter::save(const list<StartTimeDTO>& startTimes) {
	ostringstream oss;
	oss << startTimes;
	return oss.str();
}

///////////////////////////////////////////////////////////////////////////////

unique_ptr<DtoReader> TextReaderWriterFactory::createDtoReader() const {
	throw logic_error("TextReaderWriterFactory::createDtoReader() method not implemented");
}

unique_ptr<DtoWriter> TextReaderWriterFactory::createDtoWriter() const {
	return unique_ptr<DtoWriter>(new TextDtoWriter());
}

