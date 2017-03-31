#pragma once

class Document;


class View {
	friend Document;

	Document* document;

public:
	View() {}
	virtual ~View() {}

	virtual void init() {}
	virtual void terminate() {}

	Document* getDocument() { return document; }
	const Document* getDocument() const { return document; }
};
