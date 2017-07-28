#include "common.h"
#include "Commands_test.h"

#include "Command/CommandExecutor.h"
#include "Views/CommandLineView/Commands.h"

using namespace CommandLine;
using ::testing::_;

/*

void ProgramTest::SetUp() {
}

void ProgramTest::TearDown() {
}

TEST_F(ProgramTest, add) {
	EXPECT_CALL(document.getPrograms(), insert(_));

	ProgramAdd<IrrigationDocumentMock> command(&document);
	command.execute(Tokens());
}

TEST_F(ProgramTest, del) {
	const IdType id = 123U;

	EXPECT_CALL(document.getPrograms(), erase(id));

	ProgramDelete<IrrigationDocumentMock> command(&document);
	command.execute(Tokens({std::to_string(id)}));
}

TEST_F(ProgramTest, move) {
	const IdType id = 567;
	const size_t position = 34;

	EXPECT_CALL(document.getPrograms(), move(id, position));

	ProgramMove<IrrigationDocumentMock> command(&document);
	command.execute(Tokens({std::to_string(id), std::to_string(position)}));
}
*/
