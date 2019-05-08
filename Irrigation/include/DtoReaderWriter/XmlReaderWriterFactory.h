#pragma once
#include "DtoReaderWriter.h"
#include "XmlReader.h"
#include "XmlWriter.h"


class XmlReaderWriterFactory : public DtoReaderWriterFactory {
public:
	virtual std::shared_ptr<DtoReader> createDtoReader() const override {
		return std::make_shared<XmlReader>();
	}

	virtual std::shared_ptr<DtoWriter> createDtoWriter() const override {
		return std::make_shared<XmlWriter>();
	}
};
