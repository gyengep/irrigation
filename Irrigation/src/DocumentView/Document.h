#pragma once

#include <list>
#include <mutex>

class View;


class Document {
	mutable std::mutex mutex;
	std::list<View*> views;

public:

	Document();
	virtual ~Document();

	void addView(View* view);
	void removeView(View* view);
	void updateViews();

	virtual void save() {}
	virtual void load() {}
};
