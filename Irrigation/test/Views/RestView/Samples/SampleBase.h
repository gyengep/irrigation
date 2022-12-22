#pragma once
#include <list>
#include <memory>


namespace RestViewTestSamples {

	template<class DTO>
	class Sample {
		const DTO dto;
		const std::string xml;

	public:
		Sample(const DTO& dto, const std::string& xml) : dto(dto), xml(xml) {}
		virtual ~Sample() = default;

		DTO getDto() const { return dto; }

		const std::string getXml() const {
			return replaceIdInXml(xml, "");
		}

		const std::string getXmlWithId(unsigned id) const {
			return replaceIdInXml(xml, id);
		}

		static std::string replaceIdInXml(const std::string& xml, const unsigned id) {
			return replaceIdInXml(xml, " id=\"" + std::to_string(id) + "\"");
		}

		static std::string replaceIdInXml(const std::string& xml, const std::string& text) {
			std::string result = xml;
			const auto pos = result.find("%s");
			if (pos != std::string::npos) {
				result.replace(pos, 2, text);
			}
			return result;
		}
	};


	template <class DTO>
	class SampleList {
		const std::list<DTO> dtoList;
		const std::string xml;

	public:
		SampleList(const std::list<DTO>& dtoList, const std::string& xml) : dtoList(dtoList), xml(xml) {}
		virtual ~SampleList() = default;

		std::list<DTO> getDtoList() const { return dtoList; }
		const std::string& getXml() const { return xml; }
	};
};
