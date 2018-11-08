#include "Document.h"
#include "View.h"
#include <sstream>
#include <string>

using namespace std;


Document::Document() {
}

Document::~Document() {
	lock_guard<mutex> lockView(mtx);

	for (auto& view : views) {
		view->terminate();
	}

	views.clear();
}

void Document::addView(std::unique_ptr<View>&& view) {
	lock_guard<mutex> lockView(mtx);

	if (&view->getDocument() != this) {
		throw logic_error("Document::addView(): view->getDocument() != this");
	}

	view->initialize();
	views.push_back(move(view));
}
