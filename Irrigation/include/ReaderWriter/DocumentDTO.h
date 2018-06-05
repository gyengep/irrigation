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

	const std::list<ProgramDTO>* getPrograms() const {
		return programs.get();
	}

	void setPrograms(const std::list<ProgramDTO>* programs) {
		this->programs.reset(programs);
	}
};
