#pragma once
#include "DtoReaderWriterFactory.h"
#include "XmlReader.h"
#include "XmlWriter.h"


class XmlReaderWriterFactory : public DtoReaderWriterFactory {
public:
	virtual std::unique_ptr<DtoReader> createDtoReader() const override {
		return std::unique_ptr<DtoReader>(new XmlReader());
	}

	virtual std::unique_ptr<DtoWriter> createDtoWriter() const override {
		return std::unique_ptr<DtoWriter>(new XmlWriter());
	}
};
