#pragma once
#include <list>
#include <memory>
#include "IdType.h"
#include "DTO/ProgramDTO.h"

class Program;


class ProgramContainer {
public:
	typedef IdType									key_type;
	typedef std::shared_ptr<Program>				mapped_type;
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
	ProgramContainer() = default;
	ProgramContainer(ProgramContainer&&) = default;
	ProgramContainer(const ProgramContainer&);
	ProgramContainer(std::initializer_list<value_type> initializer);
	virtual ~ProgramContainer() = default;

	ProgramContainer& operator= (ProgramContainer&&) = delete;
	ProgramContainer& operator= (const ProgramContainer&) = delete;
	bool operator== (const ProgramContainer& other) const;

	virtual value_type& insert(const key_type& key, const mapped_type& value);
	virtual void erase(const key_type& key);

	const_iterator begin() const 	{ return container.begin(); }
	const_iterator end() const 		{ return container.end(); }
	size_t size() const 			{ return container.size(); }

	const mapped_type& at(const key_type& key) const;

	std::list<ProgramDTO> toProgramDtoList() const;
	void updateFromProgramDtoList(const std::list<ProgramDTO>& programDtoList);

	friend std::string to_string(const ProgramContainer& programContainer);
	friend std::ostream& operator<<(std::ostream& os, const ProgramContainer& programContainer);
};
