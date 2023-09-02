#include "FileWriterImpl.h"
#include "Exceptions/Exceptions.h"


FileWriterFactoryImpl::FileWriterFactoryImpl(const std::string& fileName) :
	fileName(fileName)
{
}

std::shared_ptr<FileWriter> FileWriterFactoryImpl::create(FileWriter::Type type) {
	return std::make_shared<FileWriterImpl>(fileName, type);
}

///////////////////////////////////////////////////////////////////////////////

FileWriterImpl::FileWriterImpl(const std::string& fileName, FileWriter::Type type) {

	switch (type) {
	case FileWriter::Type::APPEND:
		ofs.open(fileName, std::ofstream::out | std::ofstream::app);
		break;
	case FileWriter::Type::TRUNCATE:
		ofs.open(fileName, std::ofstream::out | std::ofstream::trunc);
		break;
	}

	if (ofs.fail()) {
		throw IOException(errno);
	}
}

FileWriterImpl::~FileWriterImpl() {
	ofs.close();
}

void FileWriterImpl::write(const std::string& text) {
	ofs << text;
	if (ofs.fail()) {
		throw IOException(errno);
	}
}
