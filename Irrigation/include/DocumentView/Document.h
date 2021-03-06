#pragma once
#include <list>
#include <memory>
#include <mutex>

class View;


class Document {
	mutable std::mutex mtx;
	std::list<std::unique_ptr<View>> views;

public:

	Document();
	virtual ~Document();

	void addView(std::unique_ptr<View>&& view);
};
