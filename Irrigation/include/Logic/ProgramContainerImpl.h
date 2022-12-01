#pragma once
#include <list>
#include <memory>
#include <string>
#include "DTO/ProgramDTO.h"
#include "ProgramContainer.h"


class ProgramContainerImpl : public ProgramContainer {

	struct findKey {
		const key_type& key;

		findKey(const key_type& key) : key(key) {}

		bool operator() (const value_type& a) {
			return (a.first == key);
		}
	};

	const std::shared_ptr<ProgramFactory> programFactory;
	container_type container;

public:
	ProgramContainerImpl(const std::shared_ptr<ProgramFactory>& programFactory);
	ProgramContainerImpl(const std::shared_ptr<ProgramFactory>& programFactory, std::initializer_list<value_type> initializer);
	ProgramContainerImpl(std::initializer_list<value_type> initializer);
	virtual ~ProgramContainerImpl() = default;

	virtual value_type& insert(const key_type& key, const mapped_type& value) override;
	virtual void erase(const key_type& key) override;

	virtual const_iterator begin() const override 	{ return container.begin(); }
	virtual const_iterator end() const override 	{ return container.end(); }
	virtual size_t size() const override 			{ return container.size(); }
	virtual bool empty() const override	 			{ return container.empty(); }

	virtual const_mapped_type at(const key_type& key) const override;
	virtual mapped_type at(const key_type& key) override;

	virtual const ProgramFactory& getProgramFactory() const override;

	virtual std::list<ProgramDTO> toProgramDtoList() const override;
	virtual void updateFromProgramDtoList(const std::list<ProgramDTO>& programDtoList) override;

	virtual std::string toString() const override;
};

///////////////////////////////////////////////////////////////////////////////

class ProgramContainerImplFactory : public ProgramContainerFactory {
	const std::shared_ptr<ProgramFactory> programFactory;

public:
	ProgramContainerImplFactory(const std::shared_ptr<ProgramFactory>& programFactory);

	virtual ~ProgramContainerImplFactory() = default;
	virtual ProgramContainerPtr create() const override;
};
