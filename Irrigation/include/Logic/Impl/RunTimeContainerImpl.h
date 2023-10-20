#pragma once
#include <list>
#include <memory>
#include <string>
#include "DTO/RunTimeDto.h"
#include "Logic/RunTimeContainer.h"


class RunTimeContainerImpl : public RunTimeContainer {
	RunTimeContainer::container_type container;

public:
	RunTimeContainerImpl(const RunTimeFactoryPtr& runTimeFactory);
	RunTimeContainerImpl(std::initializer_list<RunTimePtr> initializer);
	virtual ~RunTimeContainerImpl() = default;

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	virtual const_mapped_type at(const key_type& key) const override;
	virtual mapped_type at(const key_type& key) override;

	virtual std::list<RunTimeDto> toRunTimeDtoList() const override;
	virtual void updateFromRunTimeDtoList(const std::list<RunTimeDto>& runTimeDtoList) override;

	virtual std::string toString() const override;
	virtual DurationList toDurationList() const override;
};

///////////////////////////////////////////////////////////////////////////////

class RunTimeContainerImplFactory : public RunTimeContainerFactory {
	const RunTimeFactoryPtr runTimeFactory;

public:
	RunTimeContainerImplFactory(const RunTimeFactoryPtr& runTimeFactory);

	virtual ~RunTimeContainerImplFactory() = default;
	virtual RunTimeContainerPtr create() const override;
};
