#pragma once
#include <list>
#include <memory>
#include <string>
#include "ProgramDTO.h"



class DocumentDTO {
	std::unique_ptr<const std::list<ProgramDTO>> programs;

public:
	DocumentDTO() : programs() {
	}

	DocumentDTO(std::list<ProgramDTO>* programs) {
		if (programs != nullptr) {
			setPrograms(programs);
		}
	}

	DocumentDTO(const DocumentDTO& other) {
		if (other.hasPrograms()) {
			setPrograms(new std::list<ProgramDTO>(other.getPrograms()));
		}
	}

	bool operator== (const DocumentDTO& other) const {
		return (equalsPtr(programs.get(), other.programs.get()));
	}

	bool hasPrograms() const {
		return (programs.get() != nullptr);
	}

	const std::list<ProgramDTO>& getPrograms() const {
		if (!hasPrograms()) {
			throw std::logic_error("DocumentDTO::getPrograms()");
		}

		return *programs.get();
	}

	DocumentDTO& setPrograms(const std::list<ProgramDTO>* programs) {
		this->programs.reset(programs);
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const DocumentDTO& document) {
	    os << "DocumentDTO{";
	    PRINT_PTR(os, "programs", document.programs.get());
		os << "}";

		return os;
	}
};
