/*
 * ListWithUID.h
 *
 *  Created on: 28 Jun 2016
 *      Author: pgyenge
 */

#ifndef SORTEDMAP_H_
#define SORTEDMAP_H_


template <class KeyType, class ValueType>
class SortedMap {

public:

	typedef std::pair<KeyType, ValueType> ItemType;
	typedef std::list<ItemType> ListType;

private:

	mutable std::shared_timed_mutex mtx;
	ListType items;

public:
	SortedMap();
	~SortedMap();

	void insert(const ItemType& item);
	bool find(const KeyType& key, ValueType* value = NULL);
	bool find(const KeyType& key, const ValueType* value = NULL) const;
	bool erase(const KeyType& key, ValueType* value = NULL);

	const ListType& getItems() const;

	void lock() const;
	void unlock() const;
};

#include "SortedMap.hpp"
#endif /* SORTEDMAP_H_ */
