/*
 * Tools.h
 *
 *  Created on: 2016. jún. 30.
 *      Author: Rendszergazda
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <array>
#include <atomic>
#include <cstddef>
#include <list>
#include <utility>

#include "exception.h"


namespace tools {

template <typename K, typename V>
void push_back(std::list<std::pair<K, V>>& container, const K& uid, const V& value) {
	container.push_back(std::make_pair(uid, value));
}

template <typename K, typename V>
V erase(std::list<std::pair<K, V>>& container, const K& uid) {
	for (auto it = container.cbegin(); container.cend() != it; ++it) {
		if (it->first == uid) {
			V result = it->second;
			container.erase(it);
			return result;
		}
	}

	throw invalid_id();
}

template <typename K, typename V>
V& get(std::list<std::pair<K, V>>& container, const K& uid) {
	for (auto it = container.begin(); container.end() != it; ++it) {
		if (it->first == uid) {
			return it->second;
		}
	}

	throw invalid_id();
}

template <typename K, typename V>
const V& get(const std::list<std::pair<K, V>>& container, const K& uid) {
	for (auto it = container.cbegin(); container.cend() != it; ++it) {
		if (it->first == uid) {
			return it->second;
		}
	}

	throw invalid_id();
}

template <typename K, typename V>
void set(std::list<std::pair<K, V>>& container, const K& uid, const V& value) {
	for (auto it = container.begin(); container.end() != it; ++it) {
		if (it->first == uid) {
			it->second = value;
			return;
		}
	}

	throw invalid_id();
}

/////////////////////////////////////////////////////////////////////////

template <typename K, typename V, std::size_t S>
void set(std::array<std::pair<K, V>, S>& container, const K& uid, const V& value) {

	if (container.size() > uid) {
		container[uid].second = value;
		return;
	}

	throw invalid_id();
}

template <typename K, typename V, std::size_t S>
V get(std::array<std::pair<K, V>, S>& container, const K& uid) {

	if (container.size() > uid) {
		return container[uid].second;
	}

	throw invalid_id();
}

template <typename K, typename V, std::size_t S>
V get(const std::array<std::pair<K, V>, S>& container, const K& uid) {

	if (container.size() > uid) {
		return container[uid].second;
	}

	throw invalid_id();
}


} // namespace

#endif /* TOOLS_H_ */
