#include "DocumentDTO.h"
#include "EqualsPtr.h"
#include <stdexcept>

using namespace std;


DocumentDTO::DocumentDTO() {
}

DocumentDTO::DocumentDTO(list<ProgramDTO>* programs) {
	if (programs != nullptr) {
		setPrograms(programs);
	}
}

DocumentDTO::DocumentDTO(const DocumentDTO& other) {
	if (other.hasPrograms()) {
		setPrograms(new list<ProgramDTO>(other.getPrograms()));
	}
}

bool DocumentDTO::operator== (const DocumentDTO& other) const {
	return (equalsPtr(programs.get(), other.programs.get()));
}

bool DocumentDTO::hasPrograms() const {
	return (programs.get() != nullptr);
}

const list<ProgramDTO>& DocumentDTO::getPrograms() const {
	if (!hasPrograms()) {
		throw logic_error("DocumentDTO::getPrograms()");
	}

	return *programs.get();
}

DocumentDTO& DocumentDTO::setPrograms(const list<ProgramDTO>* programs) {
	this->programs.reset(programs);
	return *this;
}

ostream& operator<<(ostream& os, const DocumentDTO& document) {
	os << "DocumentDTO{";
	PRINT_PTR(os, "programs", document.programs.get());
	os << "}";

	return os;
}
