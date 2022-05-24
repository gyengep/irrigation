#pragma once
#include <list>
#include <memory>
#include <string>
#include "DTO/ProgramDTO.h"
#include "Utils/OstreamInsert.h"
#include "IdType.h"
#include "Program.h"


class ProgramContainer {
public:
	typedef IdType									key_type;
	typedef ProgramPtr								mapped_type;
	typedef ConstProgramPtr							const_mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::list<value_type>					container_type;
	typedef typename container_type::const_iterator const_iterator;
	typedef container_type::size_type				size_type;

	ProgramContainer() = default;
	ProgramContainer(ProgramContainer&&) = delete;
	ProgramContainer(const ProgramContainer&) = delete;
	virtual ~ProgramContainer() = default;

	ProgramContainer& operator= (ProgramContainer&&) = delete;
	ProgramContainer& operator= (const ProgramContainer&) = delete;
	bool operator== (const ProgramContainer& other) const;

	virtual value_type& insert(const key_type& key, const mapped_type& value) = 0;
	virtual void erase(const key_type& key) = 0;

	virtual const_iterator begin() const = 0;
	virtual const_iterator end() const = 0;
	virtual size_t size() const = 0;
	virtual bool empty() const = 0;

	virtual const_mapped_type at(const key_type& key) const = 0;
	virtual mapped_type at(const key_type& key) = 0;

	virtual std::list<ProgramDTO> toProgramDtoList() const = 0;
	virtual void updateFromProgramDtoList(const std::shared_ptr<ProgramFactory>& programFactory, const std::list<ProgramDTO>& programDtoList) = 0;

	virtual std::string toString() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(ProgramContainer);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<ProgramContainer> ProgramContainerPtr;

///////////////////////////////////////////////////////////////////////////////

class ProgramContainerFactory {
public:
	virtual ~ProgramContainerFactory() = default;
	virtual ProgramContainerPtr create() const = 0;
};
