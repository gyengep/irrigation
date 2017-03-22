#pragma once

class Document;


class View {
	friend Document;

	Document* document;

public:
	View() {}
	virtual ~View() {}

	virtual void start() {}
	virtual void stop(bool waitForFinish) {}
	virtual void update() {}


	Document* getDocument() { return document; }
	const Document* getDocument() const { return document; }
};
