#pragma once
#include <gmock/gmock.h>
#include "Model/IrrigationDocument.h"


class MockIrrigationDocument : public IrrigationDocument {
public:
	MOCK_CONST_METHOD0(lock, void());
	MOCK_CONST_METHOD0(unlock, void());

	MOCK_CONST_METHOD0(getProgramContainer, const ProgramContainer&());
	MOCK_METHOD0(getProgramContainer, ProgramContainer& ());

	MOCK_CONST_METHOD0(isModified, bool());
	MOCK_METHOD1(setModified, void(bool modified));

	MOCK_METHOD1(startIfScheduled, void(const LocalDateTime& localDateTime));
	MOCK_METHOD2(startProgram, void(const IdType& programId, unsigned adjustment));
	MOCK_METHOD2(startCustom, void(const DurationList& durations, unsigned adjustment));
	MOCK_METHOD0(stop, void());

	MOCK_CONST_METHOD0(toDocumentDto, DocumentDto());
	MOCK_METHOD1(updateFromDocumentDto, void(const DocumentDto& documentDto));

	MOCK_CONST_METHOD0(saveState, void());
	MOCK_METHOD0(loadState, void());

	MOCK_CONST_METHOD0(save, void());
	MOCK_METHOD0(load, void());
};
