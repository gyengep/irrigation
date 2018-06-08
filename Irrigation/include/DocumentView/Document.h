#pragma once
#include <list>
#include <mutex>
#include <string>

class View;


class Document {
	mutable std::mutex mutex;
	std::list<View*> views;

public:

	Document();
	virtual ~Document();

	void addView(View* view);
	void removeView(View* view);

	virtual void save(const std::string& fileName) const {}
	virtual void load(const std::string& fileName) {}
};
