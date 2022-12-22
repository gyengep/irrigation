#pragma once
#include <list>
#include <memory>


namespace DtoReaderWriterTestSamples {

	template<class DTO>
	class Sample {
		const DTO dto;
		const std::string xml;

	public:
		Sample(const DTO& dto, const std::string& xml) : dto(dto), xml(xml) {}
		virtual ~Sample() = default;

		DTO getDto() const { return dto; }
		const std::string& getXml() const { return xml; }
	};


	template <class DTO>
	class ContainerSample {
		const std::list<DTO> dtoList;
		const std::string xml;

	public:
		ContainerSample(const std::list<DTO>& dtoList, const std::string& xml) : dtoList(dtoList), xml(xml) {}
		virtual ~ContainerSample() = default;

		std::list<DTO> getDtoList() const { return dtoList; }
		const std::string& getXml() const { return xml; }
};

};
