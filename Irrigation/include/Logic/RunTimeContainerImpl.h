#pragma once
#include <list>
#include <memory>
#include <string>
#include "DTO/RunTimeDTO.h"
#include "RunTimeContainer.h"


class RunTimeContainerImpl : public RunTimeContainer {
	RunTimeContainer::container_type container;

public:
	RunTimeContainerImpl(const std::shared_ptr<RunTimeFactory>& runTimeFactory);
	RunTimeContainerImpl(std::initializer_list<RunTimePtr> initializer);
	virtual ~RunTimeContainerImpl() = default;

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	virtual const_mapped_type at(const key_type& key) const override;
	virtual mapped_type at(const key_type& key) override;

	virtual std::list<RunTimeDTO> toRunTimeDtoList() const override;
	virtual void updateFromRunTimeDtoList(const std::list<RunTimeDTO>& runTimeDtoList) override;

	virtual std::string toString() const override;
	virtual std::list<std::chrono::seconds> toDurations() const override;
};

///////////////////////////////////////////////////////////////////////////////

class RunTimeContainerImplFactory : public RunTimeContainerFactory {
	const std::shared_ptr<RunTimeFactory> runTimeFactory;

public:
	RunTimeContainerImplFactory(const std::shared_ptr<RunTimeFactory>& runTimeFactory);

	virtual ~RunTimeContainerImplFactory() = default;
	virtual RunTimeContainerPtr create() const override;
};
