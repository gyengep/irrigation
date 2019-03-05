#pragma once
#include "DtoReaderWriter/XmlReaderWriterFactory.h"
#include "WebServer/ErrorWriter.h"


class XmlErrorWriter : public ErrorWriter {
public:
	virtual ~XmlErrorWriter() = default;
	virtual std::string to(const std::string& errorMessage, unsigned errorCode) override;
};


class XmlErrorWriterFactory : public ErrorWriterFactory {
public:
	virtual ~XmlErrorWriterFactory() = default;
	virtual std::unique_ptr<ErrorWriter> create() const override;
};

