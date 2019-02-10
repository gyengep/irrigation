#pragma once
#include <cstddef>
#include <list>
#include <memory>
#include "IdType.h"
#include "DTO/StartTimeDTO.h"

class StartTime;


class StartTimeContainer {
public:
	typedef IdType									key_type;
	typedef std::shared_ptr<StartTime>				mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::list<value_type>					container_type;
	typedef typename container_type::const_iterator const_iterator;
	typedef container_type::size_type				size_type;

private:

	struct findKey {
		const key_type& key;

		findKey(const key_type& key) : key(key) {}

		bool operator() (const value_type& a) {
			return (a.first == key);
		}
	};

	container_type container;

public:
	StartTimeContainer() = default;
	StartTimeContainer(StartTimeContainer&&) = default;
	StartTimeContainer(const StartTimeContainer& other);
	StartTimeContainer(std::initializer_list<value_type> initializer);
	virtual ~StartTimeContainer() = default;

	StartTimeContainer& operator= (StartTimeContainer&&) = delete;
	StartTimeContainer& operator= (const StartTimeContainer&) = delete;
	bool operator== (const StartTimeContainer& other) const;

	virtual value_type& insert(const key_type& key, const mapped_type& value);
	virtual void erase(const key_type& key);
	virtual void sort();

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	const mapped_type& at(const key_type& key) const;

	std::list<StartTimeDTO> toStartTimeDtoList() const;
	void updateFromStartTimeDtoList(const std::list<StartTimeDTO>& startTimeDtoList);

	friend std::string to_string(const StartTimeContainer& startTimeContainer);
	friend std::ostream& operator<<(std::ostream& os, const StartTimeContainer& startTimeContainer);
};
