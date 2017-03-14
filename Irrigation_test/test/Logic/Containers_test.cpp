#include "common.h"
#include "Logic/Containers.h"
#include "Logic/Program.h"
#include "Utils/UniqueID.h"


TEST(RunTimeContainer, size) {
	RunTimeContainer runTimes;
	EXPECT_EQ(ZONE_COUNT, runTimes.size());
}

TEST(RunTimeContainer, set) {
	RunTimeContainer runTimes;
	unsigned i;

	for (i = 0; i < ZONE_COUNT; i++) {
		runTimes.at(i) = i * 10;
	}

	i = 0;
	for (auto it = runTimes.begin(); runTimes.end() != it; ++it, ++i) {
		EXPECT_EQ(i, it->first);
		EXPECT_EQ(i * 10, it->second);
	}
}

TEST(RunTimeContainer, setInvalid) {
	RunTimeContainer runTimes;
	EXPECT_THROW(runTimes.at(runTimes.size()), InvalidRunTimeIdException);
}

TEST(RunTimeContainer, setAt) {
	RunTimeContainer runTimes;
	unsigned i;

	for (i = 0; i < ZONE_COUNT; i++) {
		runTimes.at(i) = i * 10;
	}

	i = 0;
	for (auto it = runTimes.begin(); runTimes.end() != it; ++it, ++i) {
		EXPECT_EQ(i, it->first);
		EXPECT_EQ(i * 10, it->second);
	}
}

TEST(RunTimeContainer, setAtInvalid) {
	RunTimeContainer runTimes;
	EXPECT_THROW(runTimes.at(runTimes.size()), InvalidRunTimeIdException);
}

TEST(RunTimeContainer, get) {
	RunTimeContainer runTimes;
	unsigned i;

	for (i = 0; i < ZONE_COUNT; i++) {
		EXPECT_NO_THROW(runTimes[i]);
		EXPECT_EQ(0, runTimes[i]);
	}

	for (i = 0; i < ZONE_COUNT; i++) {
		runTimes.at(i) = i * 10;
	}

	for (i = 0; i < ZONE_COUNT; i++) {
		EXPECT_EQ(i * 10, runTimes.at(i));
	}
}

TEST(RunTimeContainer, getInvalid) {
	RunTimeContainer runTimes;

	EXPECT_THROW({
		unsigned a = runTimes[runTimes.size()];
		a = a;
	}, InvalidRunTimeIdException);

	EXPECT_THROW({
		unsigned a = runTimes.at(runTimes.size());
		a = a;
	}, InvalidRunTimeIdException);
}

//////////////////////////////////////////////////////////////////////

typedef std::list<std::pair<IdType, StartTime>> StartTimeList;

StartTimeList createCopy(const StartTimeContainer& startTimes) {
	StartTimeList startTimeCopy;

	for (auto it = startTimes.begin(); startTimes.end() != it; ++it) {
		startTimeCopy.push_back(*it);
	}

	return startTimeCopy;
}

TEST(StartTimeContainer, insert) {
	StartTimeContainer startTimes;

	IdType id1 = startTimes.insert(StartTime(12, 20));
	EXPECT_EQ(
		StartTimeList({ { id1, StartTime(12, 20) } }),
		createCopy(startTimes));

	IdType id2 = startTimes.insert(StartTime(10, 10));
	EXPECT_EQ(
		StartTimeList({ { id2, StartTime(10, 10) }, { id1, StartTime(12, 20) } }),
		createCopy(startTimes));

	IdType id3 = startTimes.insert(StartTime(10, 15));
	EXPECT_EQ(
		StartTimeList({ { id2, StartTime(10, 10) }, { id3, StartTime(10, 15) }, { id1, StartTime(12, 20) } }),
		createCopy(startTimes));
}

TEST(StartTimeContainer, erase) {
	StartTimeContainer startTimes;

	IdType id1 = startTimes.insert(StartTime(10, 10));
	IdType id2 = startTimes.insert(StartTime(13, 40));
	IdType id3 = startTimes.insert(StartTime(11, 20));
	IdType id4 = startTimes.insert(StartTime(12, 30));

	// Delete from middle
	startTimes.erase(id2);
	EXPECT_EQ(
		StartTimeList({ { id1, StartTime(10, 10) }, { id3, StartTime(11, 20) }, { id4, StartTime(12, 30) } }),
		createCopy(startTimes));

	// Delete first
	startTimes.erase(id1);
	EXPECT_EQ(
		StartTimeList({ { id3, StartTime(11, 20) }, { id4, StartTime(12, 30) } }),
		createCopy(startTimes));

	// Delete last
	startTimes.erase(id3);
	EXPECT_EQ(
		StartTimeList({ { id4, StartTime(12, 30) } }),
		createCopy(startTimes));

	// Delete last
	startTimes.erase(id4);
	EXPECT_EQ(
		StartTimeList(),
		createCopy(startTimes));
}

TEST(StartTimeContainer, eraseInvalid) {
	StartTimeContainer startTimes;
	startTimes.insert(StartTime(10, 10));

	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW(startTimes.erase(invalidId), InvalidStartTimeIdException);
}

TEST(StartTimeContainer, modify) {
	StartTimeContainer startTimes;

	IdType id1 = startTimes.insert(StartTime(10, 10));
	IdType id2 = startTimes.insert(StartTime(13, 40));
	IdType id3 = startTimes.insert(StartTime(11, 20));
	IdType id4 = startTimes.insert(StartTime(12, 30));

	startTimes.modify(id2, StartTime(13, 31));

	{
		StartTimeList expectedStartTimes {
			{ id1, StartTime(10, 10) },
			{ id3, StartTime(11, 20) },
			{ id4, StartTime(12, 30) },
			{ id2, StartTime(13, 31) }
		};

		EXPECT_EQ(expectedStartTimes, createCopy(startTimes));
	}

	startTimes.modify(id3, StartTime(23, 32));

	{
		StartTimeList expectedStartTimes {
			{ id1, StartTime(10, 10) },
			{ id4, StartTime(12, 30) },
			{ id2, StartTime(13, 31) },
			{ id3, StartTime(23, 32) }
		};

		EXPECT_EQ(expectedStartTimes, createCopy(startTimes));
	}
}

TEST(StartTimeContainer, modifyInvalid) {
	StartTimeContainer startTimes;
	startTimes.insert(StartTime(10, 10));

	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW(startTimes.modify(invalidId, StartTime(13, 31)), InvalidStartTimeIdException);
}

TEST(StartTimeContainer, get) {
	StartTimeContainer startTimes;

	IdType id1 = startTimes.insert(StartTime(10, 10));
	IdType id2 = startTimes.insert(StartTime(13, 40));
	IdType id3 = startTimes.insert(StartTime(11, 20));
	IdType id4 = startTimes.insert(StartTime(12, 30));

	EXPECT_EQ(StartTime(10, 10), startTimes.at(id1));
	EXPECT_EQ(StartTime(13, 40), startTimes.at(id2));
	EXPECT_EQ(StartTime(11, 20), startTimes.at(id3));
	EXPECT_EQ(StartTime(12, 30), startTimes.at(id4));

	EXPECT_EQ(StartTime(10, 10), startTimes[id1]);
	EXPECT_EQ(StartTime(13, 40), startTimes[id2]);
	EXPECT_EQ(StartTime(11, 20), startTimes[id3]);
	EXPECT_EQ(StartTime(12, 30), startTimes[id4]);
}

TEST(StartTimeContainer, getInvalid) {
	StartTimeContainer startTimes;
	startTimes.insert(StartTime(10, 10));

	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW(startTimes.at(invalidId), InvalidStartTimeIdException);
	EXPECT_THROW(startTimes[invalidId], InvalidStartTimeIdException);
}

//////////////////////////////////////////////////////////////////////

typedef std::list<std::pair<IdType, Program*>> ProgramList;

class ProgramContainerCallback {
	ProgramList programList;
public:

	void callback(IdType id, LockedProgram program) {
		programList.push_back(std::make_pair(id, &(*program)));
	}

	const ProgramList& getProgramList() const {
		return programList;
	}
};

ProgramList createCopy(const ProgramContainer& programs) {
	ProgramContainerCallback programContainerCallback;
	auto f = std::bind(&ProgramContainerCallback::callback, &programContainerCallback, std::placeholders::_1, std::placeholders::_2);
	programs.iterate(f);

	return programContainerCallback.getProgramList();
}

TEST(ProgramContainer, insert) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();

	IdType id1 = programs.insert(program1);

	EXPECT_EQ(
		ProgramList({ { id1, program1 } }),
		createCopy(programs));

	IdType id2 = programs.insert(program2);

	EXPECT_EQ(
		ProgramList({ { id1, program1 }, { id2, program2 } }),
		createCopy(programs));
}

TEST(ProgramContainer, erase) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();
	Program* program4 = new Program();

	IdType id1 = programs.insert(program1);
	IdType id2 = programs.insert(program2);
	IdType id3 = programs.insert(program3);
	IdType id4 = programs.insert(program4);

	EXPECT_EQ(
		ProgramList({ { id1, program1 }, { id2, program2 }, { id3, program3 }, { id4, program4 } }),
		createCopy(programs));

	// Erase from middle
	programs.erase(id2);

	EXPECT_EQ(
		ProgramList({ { id1, program1 }, { id3, program3 }, { id4, program4 } }),
		createCopy(programs));

	// Erase first
	programs.erase(id1);

	EXPECT_EQ(
		ProgramList({ { id3, program3 }, { id4, program4 } }),
		createCopy(programs));

	// Erase last
	programs.erase(id4);

	EXPECT_EQ(
		ProgramList({ { id3, program3 } }),
		createCopy(programs));

	// Erase last
	programs.erase(id3);

	EXPECT_EQ(
		ProgramList(),
		createCopy(programs));
}

TEST(ProgramContainer, eraseInvalid) {
	ProgramContainer programs;
	programs.insert(new Program());

	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW(programs.erase(invalidId), InvalidProgramIdException);
}

TEST(ProgramContainer, moveFirstFirst) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs.insert(program1);
	IdType id2 = programs.insert(program2);
	IdType id3 = programs.insert(program3);

	programs.move(id1, 0);

	{
		ProgramList expectedPrograms {
			{ id1, program1 },
			{ id2, program2 },
			{ id3, program3 }
		};

		EXPECT_EQ(expectedPrograms, createCopy(programs));
	}
}

TEST(ProgramContainer, moveFirstMiddle) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs.insert(program1);
	IdType id2 = programs.insert(program2);
	IdType id3 = programs.insert(program3);

	programs.move(id1, 1);

	{
		ProgramList expectedPrograms {
			{ id2, program2 },
			{ id1, program1 },
			{ id3, program3 }
		};

		EXPECT_EQ(expectedPrograms, createCopy(programs));
	}
}

TEST(ProgramContainer, moveFirstLast) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs.insert(program1);
	IdType id2 = programs.insert(program2);
	IdType id3 = programs.insert(program3);

	programs.move(id1, 2);

	{
		ProgramList expectedPrograms {
			{ id2, program2 },
			{ id3, program3 },
			{ id1, program1 }
		};

		EXPECT_EQ(expectedPrograms, createCopy(programs));
	}
}

TEST(ProgramContainer, moveMiddleFirst) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs.insert(program1);
	IdType id2 = programs.insert(program2);
	IdType id3 = programs.insert(program3);

	programs.move(id2, 0);

	{
		ProgramList expectedPrograms {
			{ id2, program2 },
			{ id1, program1 },
			{ id3, program3 }
		};

		EXPECT_EQ(expectedPrograms, createCopy(programs));
	}
}

TEST(ProgramContainer, moveMiddleMiddle) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs.insert(program1);
	IdType id2 = programs.insert(program2);
	IdType id3 = programs.insert(program3);

	programs.move(id2, 1);

	{
		ProgramList expectedPrograms {
			{ id1, program1 },
			{ id2, program2 },
			{ id3, program3 }
		};

		EXPECT_EQ(expectedPrograms, createCopy(programs));
	}
}

TEST(ProgramContainer, moveMiddleLast) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs.insert(program1);
	IdType id2 = programs.insert(program2);
	IdType id3 = programs.insert(program3);

	programs.move(id2, 2);

	{
		ProgramList expectedPrograms {
			{ id1, program1 },
			{ id3, program3 },
			{ id2, program2 }
		};

		EXPECT_EQ(expectedPrograms, createCopy(programs));
	}
}

TEST(ProgramContainer, moveLastFirst) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs.insert(program1);
	IdType id2 = programs.insert(program2);
	IdType id3 = programs.insert(program3);

	programs.move(id3, 0);

	{
		ProgramList expectedPrograms {
			{ id3, program3 },
			{ id1, program1 },
			{ id2, program2 }
		};

		EXPECT_EQ(expectedPrograms, createCopy(programs));
	}
}

TEST(ProgramContainer, moveLastMiddle) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs.insert(program1);
	IdType id2 = programs.insert(program2);
	IdType id3 = programs.insert(program3);

	programs.move(id3, 1);

	{
		ProgramList expectedPrograms {
			{ id1, program1 },
			{ id3, program3 },
			{ id2, program2 }
		};

		EXPECT_EQ(expectedPrograms, createCopy(programs));
	}
}

TEST(ProgramContainer, moveLastLast) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs.insert(program1);
	IdType id2 = programs.insert(program2);
	IdType id3 = programs.insert(program3);

	programs.move(id3, 2);

	{
		ProgramList expectedPrograms {
			{ id1, program1 },
			{ id2, program2 },
			{ id3, program3 }
		};

		EXPECT_EQ(expectedPrograms, createCopy(programs));
	}
}

TEST(ProgramContainer, moveInvalid) {
	ProgramContainer programs;

	IdType id1 = programs.insert(new Program());
	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW(programs.move(id1, programs.size()), std::out_of_range);
	EXPECT_THROW(programs.move(invalidId, 0), InvalidProgramIdException);
}

TEST(ProgramContainer, get) {
	ProgramContainer programs;

	Program* program1 = new Program();
	Program* program2 = new Program();

	IdType id1 = programs.insert(program1);
	IdType id2 = programs.insert(program2);

	EXPECT_EQ(program1, &(*programs[id1]));
	EXPECT_EQ(program2, &(*programs[id2]));
}

TEST(ProgramContainer, getInvalid) {
	ProgramContainer programs;
	programs.insert(new Program());

	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW({
		LockedProgram program = programs.at(invalidId);
	}, InvalidProgramIdException);

	EXPECT_THROW({
		LockedProgram program = programs[invalidId];
	}, InvalidProgramIdException);
}
