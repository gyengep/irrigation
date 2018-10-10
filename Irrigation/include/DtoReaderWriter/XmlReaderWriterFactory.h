#pragma once
#include "DtoReaderWriterFactory.h"
#include "XmlReader.h"
#include "XmlWriter.h"


class XmlReaderWriterFactory : public DtoReaderWriterFactory {
public:
	virtual DtoReader* createDtoReader() const override { return new XmlReader(); }
	virtual DtoWriter* createDtoWriter() const override { return new XmlWriter(); }
};
