#pragma once
#include <list>
#include <memory>
#include <string>
#include "DTO/ProgramDTO.h"
#include "Logic/ProgramContainer.h"


class ProgramContainerImpl : public ProgramContainer {

	struct findKey {
		const key_type& key;

		findKey(const key_type& key) : key(key) {}

		bool operator() (const value_type& a) {
			return (a.first == key);
		}
	};

	const ProgramFactoryPtr programFactory;
	container_type container;

	value_type& createUpdateAndInsert(const IdType& id, const ProgramDTO& programDto);

public:
	ProgramContainerImpl(const ProgramFactoryPtr& programFactory);
	ProgramContainerImpl(std::initializer_list<value_type> initializer);
	virtual ~ProgramContainerImpl() = default;

	virtual value_type& insert(const key_type& key, const mapped_type& value) override;	// throw AlreadyExistException
	virtual void erase(const key_type& key) override;									// throw NoSuchElementException

	virtual const_iterator begin() const override 	{ return container.begin(); }
	virtual const_iterator end() const override 	{ return container.end(); }
	virtual size_t size() const override 			{ return container.size(); }
	virtual bool empty() const override	 			{ return container.empty(); }

	virtual const_mapped_type at(const key_type& key) const override;					// throw NoSuchElementException
	virtual mapped_type at(const key_type& key) override;								// throw NoSuchElementException

	virtual std::list<ProgramDTO> toProgramDtoList() const override;
	virtual void updateFromProgramDtoList(const std::list<ProgramDTO>& programDtoList) override;
	virtual value_type& createFromProgramDto(const ProgramDTO& programDto) override;

	virtual std::string toString() const override;
};
