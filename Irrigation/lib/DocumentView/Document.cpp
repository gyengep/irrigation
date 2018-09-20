#include "Document.h"
#include "View.h"
#include <sstream>
#include <string>

using namespace std;


Document::Document() {
}

Document::~Document() {
	lock_guard<std::mutex> lockView(mutex);

	for (auto it = views.begin(); views.end() != it; ++it) {
		(*it)->terminate();
	}

	views.clear();
}

void Document::addView(View* view) {
	unique_ptr<View> viewPtr(view);
	lock_guard<std::mutex> lockView(mutex);

	if (&view->getDocument() != this) {
		throw logic_error("Document::addView()  view->getDocument() != this");
	}

	views.push_back(move(viewPtr));
	view->initialize();
}
