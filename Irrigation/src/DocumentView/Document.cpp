#include "common.h"
#include "Document.h"
#include "View.h"


Document::Document() {
}

Document::~Document() {
	std::lock_guard<std::mutex> lockView(mutex);

	for (auto it = views.begin(); views.end() != it; ++it) {
		(*it)->stop(false);
	}

	for (auto it = views.begin(); views.end() != it; ++it) {
		(*it)->stop(true);
		delete (*it);
	}

	views.clear();
}

void Document::addView(View* view) {
	std::lock_guard<std::mutex> lockView(mutex);
	views.push_back(view);
	view->document = this;
	view->start();
}

void Document::removeView(View* view) {
	std::lock_guard<std::mutex> lockView(mutex);
	views.remove(view);
	view->stop(true);
	delete view;
}

void Document::updateViews() {
	std::lock_guard<std::mutex> lockView(mutex);
	for (auto it = views.begin(); views.end() != it; ++it) {
		(*it)->update();
	}
}
