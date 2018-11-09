#pragma once
#include <list>
#include <memory>
#include <ostream>
#include "ProgramDTO.h"


class DocumentDTO {
	std::unique_ptr<const std::list<ProgramDTO>> programs;

public:
	DocumentDTO() = default;
	DocumentDTO(DocumentDTO&& other) = default;
	DocumentDTO(const DocumentDTO& other);
	DocumentDTO(std::list<ProgramDTO>&& programs);

	DocumentDTO& operator= (DocumentDTO&&) = delete;
	DocumentDTO& operator= (const DocumentDTO&) = delete;
	bool operator== (const DocumentDTO& other) const;

	bool hasPrograms() const;
	const std::list<ProgramDTO>& getPrograms() const;
	DocumentDTO& setPrograms(std::list<ProgramDTO>&& programs);

	friend std::ostream& operator<<(std::ostream& os, const DocumentDTO& document);
};
