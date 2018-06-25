#pragma once
#include <list>
#include <memory>
#include <mutex>

class View;


class Document {
	mutable std::mutex mutex;
	std::list<std::unique_ptr<View>> views;

public:

	Document();
	virtual ~Document();

	void addView(View* view);
	void removeView(View* view);
};
