#include "DocumentDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


DocumentDTO::DocumentDTO(const DocumentDTO& other) {
	if (other.hasPrograms()) {
		setPrograms(list<ProgramDTO>(other.getPrograms()));
	}
}

DocumentDTO::DocumentDTO(list<ProgramDTO>&& programs) {
	setPrograms(move(programs));
}

bool DocumentDTO::operator== (const DocumentDTO& other) const {
	return (equalsPtr(programs.get(), other.programs.get()));
}

bool DocumentDTO::hasPrograms() const {
	return (programs.get() != nullptr);
}

const list<ProgramDTO>& DocumentDTO::getPrograms() const {
	if (!hasPrograms()) {
		throw logic_error("DocumentDTO::getPrograms(): !hasPrograms()");
	}

	return *programs.get();
}

DocumentDTO& DocumentDTO::setPrograms(list<ProgramDTO>&& programs) {
	this->programs.reset(new list<ProgramDTO>(move(programs)));
	return *this;
}

ostream& operator<<(ostream& os, const DocumentDTO& document) {
	os << "DocumentDTO{";
	PRINT_PTR(os, "programs", document.programs.get());
	os << "}";

	return os;
}
