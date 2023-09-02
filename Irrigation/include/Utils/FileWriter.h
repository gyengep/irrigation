#pragma once
#include <memory>
#include <string>


class FileWriter {
public:
	enum class Type { TRUNCATE, APPEND };

	virtual ~FileWriter() = default;
	virtual void write(const std::string& text) = 0;
};

///////////////////////////////////////////////////////////////////////////////

class FileWriterFactory {
public:
	virtual ~FileWriterFactory() = default;
	virtual std::shared_ptr<FileWriter> create(FileWriter::Type) = 0;
};
