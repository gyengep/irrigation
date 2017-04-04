#pragma once


template <class T> class Factory {
public:
	virtual ~Factory() {}
	virtual T* create() = 0;
};


template <class T> class FactoryHolder {
	static std::unique_ptr<Factory<T>> factory;

public:
	static T* create() { return factory->create(); }
	static void setFactory(Factory<T>* newFactory) { factory.reset(newFactory); }
};
