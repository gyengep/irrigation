#pragma once
#include <list>
#include <memory>
#include <string>
#include "DTO/StartTimeDTO.h"
#include "Logic/StartTimeContainer.h"


class StartTimeContainerImpl : public StartTimeContainer {

	struct findKey {
		const key_type& key;

		findKey(const key_type& key) : key(key) {}

		bool operator() (const value_type& a) {
			return (a.first == key);
		}
	};

	const std::shared_ptr<StartTimeFactory> startTimeFactory;
	container_type container;

	value_type& createUpdateAndInsert(const IdType& id, const StartTimeDTO& startTimeDto);

public:
	StartTimeContainerImpl(const std::shared_ptr<StartTimeFactory>& startTimeFactory);
	StartTimeContainerImpl(std::initializer_list<value_type> initializer);
	virtual ~StartTimeContainerImpl() = default;

	virtual const_iterator begin() const override 	{ return container.begin(); }
	virtual const_iterator end() const override		{ return container.end(); }
	virtual size_t size() const override 			{ return container.size(); }
	virtual bool empty() const override	 			{ return container.empty(); }

	virtual value_type& insert(const key_type& key, const mapped_type& value) override;		// throw AlreadyExistException
	virtual void erase(const key_type& key) override;										// throw NoSuchElementException
	virtual void sort() override;

	virtual const_mapped_type at(const key_type& key) const override;						// throw NoSuchElementException
	virtual mapped_type at(const key_type& key) override;									// throw NoSuchElementException

	virtual std::list<StartTimeDTO> toStartTimeDtoList() const override;
	virtual void updateFromStartTimeDtoList(const std::list<StartTimeDTO>& startTimeDtoList) override;
	virtual value_type& createFromStartTimeDto(const StartTimeDTO& startTimeDto) override;

	virtual std::string toString() const override;
};

///////////////////////////////////////////////////////////////////////////////

class StartTimeContainerImplFactory : public StartTimeContainerFactory {
	const std::shared_ptr<StartTimeFactory> startTimeFactory;

public:
	StartTimeContainerImplFactory(const std::shared_ptr<StartTimeFactory>& startTimeFactory);

	virtual ~StartTimeContainerImplFactory() = default;
	virtual StartTimeContainerPtr create() const override;
};
