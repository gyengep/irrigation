/*
 * ListWithUID.cpp
 *
 *  Created on: 28 Jun 2016
 *      Author: pgyenge
 */

#include "SortedMap.h"

template <class KeyType, class ValueType>
SortedMap<KeyType,ValueType>::SortedMap() {

}

template <class KeyType, class ValueType>
SortedMap<KeyType,ValueType>::~SortedMap() {

}

template <class KeyType, class ValueType>
void SortedMap<KeyType,ValueType>::insert(const ItemType& item) {
	std::lock_guard<std::shared_timed_mutex> lock(mtx);
	items.push_back(item);
}

template <class KeyType, class ValueType>
bool SortedMap<KeyType,ValueType>::erase(const KeyType& key, ValueType* value /*= NULL*/) {
/*
	std::lock_guard<std::shared_timed_mutex> lock(mtx);

	for (auto it = items.begin(); items.end() != it; ++it) {
		if (key == (*it).first) {
			if (value) {
				*value = (*it).second;
			}
			items.erase(it);
			return true;
		}
	}
*/
	return false;
}

template <class KeyType, class ValueType>
bool SortedMap<KeyType,ValueType>::find(const KeyType& key, ValueType* value /*= NULL*/) {
	/*
	// TODO mtx.lock_shared();
	std::lock_guard<std::shared_timed_mutex> lock(mtx);

	for (auto it = items.begin(); items.end() != it; ++it) {
		if (key == (*it).first) {
			if (value) {
				*value = (*it).second;
			}
			return true;
		}
	}
*/
	return false;
}

template <class KeyType, class ValueType>
bool SortedMap<KeyType,ValueType>::find(const KeyType& key, const ValueType* value /*= NULL*/) const {
/*
	// TODO mtx.lock_shared();
	std::lock_guard<std::shared_timed_mutex> lock(mtx);

	for (auto it = items.cbegin(); items.cend() != it; ++it) {
		if (key == (*it).first) {
			if (value) {
				*value = (*it).second;
			}
			return true;
		}
	}
*/
	return false;
}

template <class KeyType, class ValueType>
const typename SortedMap<KeyType,ValueType>::ListType& SortedMap<KeyType,ValueType>::getItems() const {
	mtx.lock_shared();
	return items;
}

template <class KeyType, class ValueType>
void SortedMap<KeyType,ValueType>::unlock() const {
	mtx.unlock_shared();
}
