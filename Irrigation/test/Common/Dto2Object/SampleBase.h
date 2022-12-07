#pragma once
#include <list>
#include <memory>


namespace Dto2ObjectTest {

	template<class OBJECT, class DTO>
	class ObjectSample {
		const std::shared_ptr<OBJECT> objectPtr;
		const DTO dto;

	public:
		ObjectSample(const std::shared_ptr<OBJECT>& objectPtr, const DTO& dto) : objectPtr(objectPtr), dto(dto) {}
		virtual ~ObjectSample() = default;

		const OBJECT& getObject() const { return *objectPtr; }
		const std::shared_ptr<OBJECT>& getObjectPtr() const { return objectPtr; }

		DTO getDto() const { return dto; }
	};


	template <class CONTAINER, class DTO>
	class ContainerSample {
		const std::shared_ptr<CONTAINER> containerPtr;
		const std::list<DTO> dtoList;

	public:
		ContainerSample(const std::shared_ptr<CONTAINER>& containerPtr, const std::list<DTO>& dtoList) : containerPtr(containerPtr), dtoList(dtoList) {}
		virtual ~ContainerSample() = default;

		const CONTAINER& getContainer() const { return *containerPtr; }
		const std::shared_ptr<CONTAINER>& getContainerPtr() const { return containerPtr; }

		std::list<DTO> getDtoList() const { return dtoList; }
	};
};
