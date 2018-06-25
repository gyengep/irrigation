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
	lock_guard<std::mutex> lockView(mutex);
	views.push_back(unique_ptr<View>(view));
	view->document = this;
	view->init();
}

void Document::removeView(View* view) {
	lock_guard<std::mutex> lockView(mutex);

	for (auto it = views.begin(); views.end() != it; ++it) {
		if (it->get() == view) {
			(*it)->terminate();
			views.erase(it);
			return;
		}
	}

	ostringstream o;
	o << "Document::removeView(): " << view << " doesn't exist";
	throw logic_error(o.str());
}

