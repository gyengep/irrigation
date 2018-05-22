#include "Document.h"
#include "View.h"

using namespace std;


Document::Document() {
}

Document::~Document() {
	lock_guard<std::mutex> lockView(mutex);

	for (auto it = views.begin(); views.end() != it; ++it) {
		(*it)->terminate();
	}

	for (auto it = views.begin(); views.end() != it; ++it) {
		delete (*it);
	}

	views.clear();
}

void Document::addView(View* view) {
	lock_guard<std::mutex> lockView(mutex);
	views.push_back(view);
	view->document = this;
	view->init();
}

void Document::removeView(View* view) {
	lock_guard<std::mutex> lockView(mutex);
	views.remove(view);
	view->terminate();
	delete view;
}

