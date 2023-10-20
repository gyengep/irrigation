#pragma once
#include <list>
#include <memory>
#include "DTO/RunTimeDto.h"
#include "DtoReaderWriter/XmlReader.h"
#include "Logic/IdType.h"


struct IrrigationActionDto {
	std::unique_ptr<std::string> action;
	std::unique_ptr<unsigned> adjustment;
	std::unique_ptr<IdType> programId;
	std::unique_ptr<RunTimeDtoList> runTimeDtoList;
};

class XmlIrrigationActionReader: public XmlReader {
public:

	XmlIrrigationActionReader();
	virtual ~XmlIrrigationActionReader();

	IrrigationActionDto load(const std::string& text) const;
};
