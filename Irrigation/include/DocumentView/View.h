#pragma once

class Document;


class View {
	Document& document;

public:
	View(Document& document) : document(document) {}
	virtual ~View() = default;

	virtual void initialize() {}
	virtual void terminate() {}

	Document& getDocument() { return document; }
	const Document& getDocument() const { return document; }
};
