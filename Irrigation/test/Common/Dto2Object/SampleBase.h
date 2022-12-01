#pragma once
#include <list>
#include <memory>


namespace Dto2ObjectTest {

	template<class OBJECT, class DTO>
	class ObjectSample {
		std::shared_ptr<OBJECT> objectPtr;
		DTO dto;

	public:
		ObjectSample(const std::shared_ptr<OBJECT>& objectPtr, DTO&& dto) : objectPtr(objectPtr), dto(std::move(dto)) {}
		virtual ~ObjectSample() = default;

		const OBJECT& getObject() const { return *objectPtr; }
		OBJECT& getObject() { return *objectPtr; }

		const std::shared_ptr<OBJECT>& getObjectPtr() const { return objectPtr; }
		std::shared_ptr<OBJECT>& getObjectPtr() { return objectPtr; }

		const DTO& getDto() const { return dto; }
		DTO& getDto() { return dto; }
	};


	template <class CONTAINER, class DTO>
	class ContainerSample {
		std::shared_ptr<CONTAINER> containerPtr;
		std::list<DTO> dtoList;

	public:
		ContainerSample(const std::shared_ptr<CONTAINER>& containerPtr, std::list<DTO>&& dtoList) : containerPtr(containerPtr), dtoList(std::move(dtoList)) {}
		virtual ~ContainerSample() = default;

		const CONTAINER& getContainer() const { return *containerPtr; }
		CONTAINER& getContainer() { return *containerPtr; }

		const std::shared_ptr<CONTAINER>& getContainerPtr() const { return containerPtr; }
		std::shared_ptr<CONTAINER>& getContainerPtr() { return containerPtr; }

		const std::list<DTO>& getDtoList() const { return dtoList; }
		std::list<DTO>& getDtoList() { return dtoList; }
	};
};
