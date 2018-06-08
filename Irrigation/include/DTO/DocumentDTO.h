#pragma once
#include <list>
#include <memory>
#include <ostream>
#include "ProgramDTO.h"



class DocumentDTO {
	std::unique_ptr<const std::list<ProgramDTO>> programs;

public:
	DocumentDTO();
	DocumentDTO(const DocumentDTO& other);
	DocumentDTO(std::list<ProgramDTO>* programs);

	bool operator== (const DocumentDTO& other) const;

	bool hasPrograms() const;
	const std::list<ProgramDTO>& getPrograms() const;
	DocumentDTO& setPrograms(const std::list<ProgramDTO>* programs);

	friend std::ostream& operator<<(std::ostream& os, const DocumentDTO& document);
};
