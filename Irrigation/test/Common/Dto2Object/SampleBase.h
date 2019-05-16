#pragma once
#include <list>
#include <memory>


namespace Dto2ObjectTest {

	template<class OBJECT, class DTO>
	class ObjectSample {
		const std::shared_ptr<OBJECT> object;
		const DTO dto;

	public:
		ObjectSample(const OBJECT& object, const DTO& dto) : object(new OBJECT(object)), dto(dto) {}
		ObjectSample(const std::shared_ptr<OBJECT> object, const DTO& dto) : object(object), dto(dto) {}

		virtual ~ObjectSample() = default;

		const std::shared_ptr<OBJECT> getObject() const { return object; }
		const DTO& getDto() const { return dto; }

		DTO getDto() { return dto; }
};


	template <class CONTAINER, class DTO>
	class ContainerSample {
		const std::shared_ptr<CONTAINER> container;
		const std::list<DTO> dtoList;

	public:
		ContainerSample(const CONTAINER& container, const std::list<DTO>& dtoList) :
			container(new CONTAINER(container)),
			dtoList(dtoList)
		{}

		virtual ~ContainerSample() = default;

		const std::shared_ptr<CONTAINER> getContainer() const { return container; }
		const std::list<DTO>& getDtoList() const { return dtoList; }
	};

};
