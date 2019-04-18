#pragma once
#include <list>
#include <memory>
#include "DTO/RunTimeDTO.h"
#include "DtoReaderWriter/XmlReader.h"
#include "Logic/IdType.h"


struct IrrigationActionDTO {
	std::unique_ptr<std::string> action;
	std::unique_ptr<unsigned> adjustment;
	std::unique_ptr<IdType> programId;
	std::unique_ptr<std::list<RunTimeDTO>> runTimeDtoList;
};

class XmlIrrigationActionReader: public XmlReader {
public:

	XmlIrrigationActionReader();
	virtual ~XmlIrrigationActionReader();

	IrrigationActionDTO load(const std::string& text) const;
};
