#pragma once
#include <list>
#include <memory>
#include <string>
#include "DTO/StartTimeDTO.h"
#include "StartTimeContainer.h"


class StartTimeContainerImpl : public StartTimeContainer {

	struct findKey {
		const key_type& key;

		findKey(const key_type& key) : key(key) {}

		bool operator() (const value_type& a) {
			return (a.first == key);
		}
	};

	container_type container;

public:
	StartTimeContainerImpl() = default;
	StartTimeContainerImpl(std::initializer_list<value_type> initializer);
	virtual ~StartTimeContainerImpl() = default;

	virtual const_iterator begin() const override 	{ return container.begin(); }
	virtual const_iterator end() const override		{ return container.end(); }
	virtual size_t size() const override 			{ return container.size(); }
	virtual bool empty() const override	 			{ return container.empty(); }

	virtual value_type& insert(const key_type& key, const mapped_type& value) override;
	virtual void erase(const key_type& key) override;
	virtual void sort() override;

	virtual const_mapped_type at(const key_type& key) const override;
	virtual mapped_type at(const key_type& key) override;

	virtual std::list<StartTimeDTO> toStartTimeDtoList() const override;
	virtual void updateFromStartTimeDtoList(const std::shared_ptr<StartTimeFactory>& startTimeFactory, const std::list<StartTimeDTO>& startTimeDtoList) override;

	virtual std::string toString() const override;
};

///////////////////////////////////////////////////////////////////////////////

class StartTimeContainerImplFactory : public StartTimeContainerFactory {
public:
	virtual ~StartTimeContainerImplFactory() = default;
	virtual StartTimeContainerPtr create() const override;
};
