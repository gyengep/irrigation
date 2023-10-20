#pragma once
#include <list>
#include <string>
#include <vector>
#include "SampleBase.h"
#include "DTO/LogEntryDto.h"


namespace DtoReaderWriterTestSamples {

	typedef Sample<LogEntryDtoList> LogEntryListSample;

	class LogEntryListSampleList : public std::vector<LogEntryListSample> {
	public:
		LogEntryListSampleList() {

			emplace_back(
				LogEntryDtoList {
				},
				"<log_entries/>"
			);

			emplace_back(
				LogEntryDtoList {
					LogEntryDto("2022.11.05 19:33:25", "0x12345678", "DEBUG", "Text1")
				},
				"<log_entries>"
					"<log_entry>"
						"<time>2022.11.05 19:33:25</time>"
						"<thread>0x12345678</thread>"
						"<level>DEBUG</level>"
						"<text>Text1</text>"
					"</log_entry>"
				"</log_entries>"
			);

			emplace_back(
				LogEntryDtoList {
					LogEntryDto("2022.11.05 19:33:25", "0x12345678", "DEBUG", "Text1"),
					LogEntryDto("2022.11.05 19:33:26", "0x12345678", "TRACE", "Text2"),
					LogEntryDto("2022.11.05 19:33:27", "0x12345678", "INFO", "Text3")
				},
				"<log_entries>"
					"<log_entry>"
						"<time>2022.11.05 19:33:25</time>"
						"<thread>0x12345678</thread>"
						"<level>DEBUG</level>"
						"<text>Text1</text>"
					"</log_entry>"
					"<log_entry>"
						"<time>2022.11.05 19:33:26</time>"
						"<thread>0x12345678</thread>"
						"<level>TRACE</level>"
						"<text>Text2</text>"
					"</log_entry>"
					"<log_entry>"
						"<time>2022.11.05 19:33:27</time>"
						"<thread>0x12345678</thread>"
						"<level>INFO</level>"
						"<text>Text3</text>"
					"</log_entry>"
				"</log_entries>"
			);
		}
	};
};
