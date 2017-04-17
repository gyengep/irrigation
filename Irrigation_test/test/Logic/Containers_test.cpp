#include "common.h"
#include "Containers_test.h"

#include "Logic/Program.h"
#include "Utils/UniqueID.h"

#include <chrono>
#include <thread>


TEST_F(RunTimeContainerTest, size) {
	EXPECT_EQ(ZONE_COUNT, runTimes->size());
}

TEST_F(RunTimeContainerTest, modify) {
	unsigned i;

	for (i = 0; i < ZONE_COUNT; i++) {
		runTimes->modify(i, i * 10);
	}

	i = 0;
	for (auto it = runTimes->begin(); runTimes->end() != it; ++it, ++i) {
		EXPECT_EQ(i, it->first);
		EXPECT_EQ(i * 10, it->second);
	}
}

TEST_F(RunTimeContainerTest, setInvalid) {
	EXPECT_THROW(runTimes->at(runTimes->size()), InvalidRunTimeIdException);
}

TEST_F(RunTimeContainerTest, setAt) {
	unsigned i;

	for (i = 0; i < ZONE_COUNT; i++) {
		runTimes->modify(i, i * 10);
	}

	i = 0;
	for (auto it = runTimes->begin(); runTimes->end() != it; ++it, ++i) {
		EXPECT_EQ(i, it->first);
		EXPECT_EQ(i * 10, it->second);
	}
}

TEST_F(RunTimeContainerTest, setAtInvalid) {
	EXPECT_THROW(runTimes->at(runTimes->size()), InvalidRunTimeIdException);
}

TEST_F(RunTimeContainerTest, get) {
	unsigned i;

	for (i = 0; i < ZONE_COUNT; i++) {
		runTimes->modify(i, i * 10);
	}

	for (i = 0; i < ZONE_COUNT; i++) {
		EXPECT_EQ(i * 10, runTimes->at(i));
	}
}

TEST_F(RunTimeContainerTest, getInvalid) {
	EXPECT_THROW({
		unsigned a = runTimes->at(runTimes->size());
		a = a;
	}, InvalidRunTimeIdException);
}

//////////////////////////////////////////////////////////////////////


typedef std::list<std::pair<IdType, StartTime>> StartTimeList;

StartTimeList createCopy(std::shared_ptr<StartTimeContainer> startTimes) {
	StartTimeList startTimeCopy;

	for (auto it = startTimes->begin(); startTimes->end() != it; ++it) {
		startTimeCopy.push_back(*it);
	}

	return startTimeCopy;
}

TEST_F(StartTimeContainerTest, insert) {
	IdType id1 = startTimes->insert(StartTime(12, 20));
	EXPECT_EQ(
		StartTimeList({ { id1, StartTime(12, 20) } }),
		createCopy(startTimes));

	IdType id2 = startTimes->insert(StartTime(10, 10));
	EXPECT_EQ(
		StartTimeList({ { id2, StartTime(10, 10) }, { id1, StartTime(12, 20) } }),
		createCopy(startTimes));

	IdType id3 = startTimes->insert(StartTime(10, 15));
	EXPECT_EQ(
		StartTimeList({ { id2, StartTime(10, 10) }, { id3, StartTime(10, 15) }, { id1, StartTime(12, 20) } }),
		createCopy(startTimes));
}

TEST_F(StartTimeContainerTest, erase) {
	IdType id1 = startTimes->insert(StartTime(10, 10));
	IdType id2 = startTimes->insert(StartTime(13, 40));
	IdType id3 = startTimes->insert(StartTime(11, 20));
	IdType id4 = startTimes->insert(StartTime(12, 30));

	// Delete from middle
	startTimes->erase(id2);
	EXPECT_EQ(
		StartTimeList({ { id1, StartTime(10, 10) }, { id3, StartTime(11, 20) }, { id4, StartTime(12, 30) } }),
		createCopy(startTimes));

	// Delete first
	startTimes->erase(id1);
	EXPECT_EQ(
		StartTimeList({ { id3, StartTime(11, 20) }, { id4, StartTime(12, 30) } }),
		createCopy(startTimes));

	// Delete last
	startTimes->erase(id3);
	EXPECT_EQ(
		StartTimeList({ { id4, StartTime(12, 30) } }),
		createCopy(startTimes));

	// Delete last
	startTimes->erase(id4);
	EXPECT_EQ(
		StartTimeList(),
		createCopy(startTimes));
}

TEST_F(StartTimeContainerTest, eraseInvalid) {
	startTimes->insert(StartTime(10, 10));

	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW(startTimes->erase(invalidId), InvalidStartTimeIdException);
}

TEST_F(StartTimeContainerTest, modify) {
	IdType id1 = startTimes->insert(StartTime(10, 10));
	IdType id2 = startTimes->insert(StartTime(13, 40));
	IdType id3 = startTimes->insert(StartTime(11, 20));
	IdType id4 = startTimes->insert(StartTime(12, 30));

	startTimes->modify(id2, StartTime(13, 31));

	{
		StartTimeList expectedStartTimes {
			{ id1, StartTime(10, 10) },
			{ id3, StartTime(11, 20) },
			{ id4, StartTime(12, 30) },
			{ id2, StartTime(13, 31) }
		};

		EXPECT_EQ(expectedStartTimes, createCopy(startTimes));
	}

	startTimes->modify(id3, StartTime(23, 32));

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

TEST_F(StartTimeContainerTest, modifyInvalid) {
	startTimes->insert(StartTime(10, 10));

	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW(startTimes->modify(invalidId, StartTime(13, 31)), InvalidStartTimeIdException);
}

TEST_F(StartTimeContainerTest, get) {
	IdType id1 = startTimes->insert(StartTime(10, 10));
	IdType id2 = startTimes->insert(StartTime(13, 40));
	IdType id3 = startTimes->insert(StartTime(11, 20));
	IdType id4 = startTimes->insert(StartTime(12, 30));

	EXPECT_EQ(StartTime(10, 10), startTimes->at(id1));
	EXPECT_EQ(StartTime(13, 40), startTimes->at(id2));
	EXPECT_EQ(StartTime(11, 20), startTimes->at(id3));
	EXPECT_EQ(StartTime(12, 30), startTimes->at(id4));
}

TEST_F(StartTimeContainerTest, getInvalid) {
	startTimes->insert(StartTime(10, 10));

	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW(startTimes->at(invalidId), InvalidStartTimeIdException);
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

ProgramList createCopy(const std::shared_ptr<ProgramContainer> programs) {
	ProgramContainerCallback programContainerCallback;
	auto f = std::bind(&ProgramContainerCallback::callback, &programContainerCallback, std::placeholders::_1, std::placeholders::_2);
	programs->iterate(f);

	return programContainerCallback.getProgramList();
}

TEST_F(ProgramContainerTest, insert) {
	Program* program1 = new Program();
	Program* program2 = new Program();

	IdType id1 = programs->insert(program1);

	EXPECT_EQ(
		ProgramList({ { id1, program1 } }),
		createCopy(programs));

	IdType id2 = programs->insert(program2);

	EXPECT_EQ(
		ProgramList({ { id1, program1 }, { id2, program2 } }),
		createCopy(programs));
}

TEST_F(ProgramContainerTest, erase) {
	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();
	Program* program4 = new Program();

	IdType id1 = programs->insert(program1);
	IdType id2 = programs->insert(program2);
	IdType id3 = programs->insert(program3);
	IdType id4 = programs->insert(program4);

	EXPECT_EQ(
		ProgramList({ { id1, program1 }, { id2, program2 }, { id3, program3 }, { id4, program4 } }),
		createCopy(programs));

	// Erase from middle
	programs->erase(id2);

	EXPECT_EQ(
		ProgramList({ { id1, program1 }, { id3, program3 }, { id4, program4 } }),
		createCopy(programs));

	// Erase first
	programs->erase(id1);

	EXPECT_EQ(
		ProgramList({ { id3, program3 }, { id4, program4 } }),
		createCopy(programs));

	// Erase last
	programs->erase(id4);

	EXPECT_EQ(
		ProgramList({ { id3, program3 } }),
		createCopy(programs));

	// Erase last
	programs->erase(id3);

	EXPECT_EQ(
		ProgramList(),
		createCopy(programs));
}

TEST_F(ProgramContainerTest, eraseInvalid) {
	programs->insert(new Program());

	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW(programs->erase(invalidId), InvalidProgramIdException);
}

TEST_F(ProgramContainerTest, moveFirstFirst) {
	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs->insert(program1);
	IdType id2 = programs->insert(program2);
	IdType id3 = programs->insert(program3);

	programs->move(id1, 0);

	EXPECT_EQ(
			ProgramList({ { id1, program1 }, { id2, program2 }, { id3, program3 } }),
			createCopy(programs));
}

TEST_F(ProgramContainerTest, moveFirstMiddle) {
	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs->insert(program1);
	IdType id2 = programs->insert(program2);
	IdType id3 = programs->insert(program3);

	programs->move(id1, 1);

	EXPECT_EQ(
			ProgramList({ { id2, program2 }, { id1, program1 }, { id3, program3 } }),
			createCopy(programs));
}

TEST_F(ProgramContainerTest, moveFirstLast) {
	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs->insert(program1);
	IdType id2 = programs->insert(program2);
	IdType id3 = programs->insert(program3);

	programs->move(id1, 2);

	EXPECT_EQ(
			ProgramList({ { id2, program2 }, { id3, program3 }, { id1, program1 } }),
			createCopy(programs));
}

TEST_F(ProgramContainerTest, moveMiddleFirst) {
	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs->insert(program1);
	IdType id2 = programs->insert(program2);
	IdType id3 = programs->insert(program3);

	programs->move(id2, 0);

	EXPECT_EQ(
			ProgramList({ { id2, program2 }, { id1, program1 }, { id3, program3 } }),
			createCopy(programs));
}

TEST_F(ProgramContainerTest, moveMiddleMiddle) {
	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs->insert(program1);
	IdType id2 = programs->insert(program2);
	IdType id3 = programs->insert(program3);

	programs->move(id2, 1);

	EXPECT_EQ(
			ProgramList({ { id1, program1 }, { id2, program2 }, { id3, program3 } }),
			createCopy(programs));
}

TEST_F(ProgramContainerTest, moveMiddleLast) {
	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs->insert(program1);
	IdType id2 = programs->insert(program2);
	IdType id3 = programs->insert(program3);

	programs->move(id2, 2);

	EXPECT_EQ(
			ProgramList({ { id1, program1 }, { id3, program3 }, { id2, program2 } }),
			createCopy(programs));
}

TEST_F(ProgramContainerTest, moveLastFirst) {
	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs->insert(program1);
	IdType id2 = programs->insert(program2);
	IdType id3 = programs->insert(program3);

	programs->move(id3, 0);

	EXPECT_EQ(
			ProgramList({ { id3, program3 }, { id1, program1 }, { id2, program2 } }),
			createCopy(programs));
}

TEST_F(ProgramContainerTest, moveLastMiddle) {
	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs->insert(program1);
	IdType id2 = programs->insert(program2);
	IdType id3 = programs->insert(program3);

	programs->move(id3, 1);

	EXPECT_EQ(
			ProgramList({ { id1, program1 }, { id3, program3 }, { id2, program2 } }),
			createCopy(programs));
}

TEST_F(ProgramContainerTest, moveLastLast) {
	Program* program1 = new Program();
	Program* program2 = new Program();
	Program* program3 = new Program();

	IdType id1 = programs->insert(program1);
	IdType id2 = programs->insert(program2);
	IdType id3 = programs->insert(program3);

	programs->move(id3, 2);

	EXPECT_EQ(
			ProgramList({ { id1, program1 }, { id2, program2 }, { id3, program3 } }),
			createCopy(programs));
}

TEST_F(ProgramContainerTest, moveInvalid) {
	IdType id1 = programs->insert(new Program());
	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW(programs->move(id1, programs->size()), std::out_of_range);
	EXPECT_THROW(programs->move(invalidId, 0), InvalidProgramIdException);
}

TEST_F(ProgramContainerTest, get) {
	Program* program1 = new Program();
	Program* program2 = new Program();

	IdType id1 = programs->insert(program1);
	IdType id2 = programs->insert(program2);

	EXPECT_EQ(program1, &(*programs->at(id1)));
	EXPECT_EQ(program2, &(*programs->at(id2)));
}

TEST_F(ProgramContainerTest, getInvalid) {
	programs->insert(new Program());

	IdType invalidId = UniqueID::getInstance().getNextId();

	EXPECT_THROW({
		LockedProgram program = programs->at(invalidId);
	}, InvalidProgramIdException);
}

void insertPrograms(std::shared_ptr<ProgramContainer> programs, size_t count, std::vector<IdType>* result) {
	for(size_t i = 0; i < count; ++i) {
		result->at(i) = programs->insert(new Program());
	}
}

TEST_F(ProgramContainerTest, concurrentInsert) {
	const size_t threadCount = 5;
	const size_t addCount = 10000;

	std::vector<IdType> results[threadCount];
    std::thread threads[threadCount];

    for (size_t i = 0; i < threadCount; ++i) {
    	results[i].resize(addCount);
    }

    for (size_t i = 0; i < threadCount; ++i) {
    	threads[i] = std::thread(insertPrograms, programs, addCount, &results[i]);
    }

    for (size_t i = 0; i < threadCount; ++i) {
    	threads[i].join();
    }

    EXPECT_EQ(threadCount * addCount, programs->size());
}

void erasePrograms(std::shared_ptr<ProgramContainer> programs, std::vector<IdType>* ids) {
	for(size_t i = 0; i < ids->size(); ++i) {
		programs->erase(ids->at(i));
	}
}

TEST_F(ProgramContainerTest, concurentErase) {
	const size_t threadCount = 5;
	const size_t addCount = 3000;

	std::vector<IdType> ids[threadCount];
    std::thread threads[threadCount];

    for (size_t i = 0; i < threadCount; ++i) {
    	ids[i].resize(addCount);

    	for (size_t t = 0; t < addCount; ++t) {
    		ids[i].at(t) = programs->insert(new Program());
    	}
    }

    for (size_t i = 0; i < threadCount; ++i) {
    	threads[i] = std::thread(erasePrograms, programs, &ids[i]);
    }

    for (size_t i = 0; i < threadCount; ++i) {
    	threads[i].join();
    }

    EXPECT_EQ(0, programs->size());
}

void getProgramAndWait(std::shared_ptr<ProgramContainer> programs, IdType id, int ms) {
	LockedProgram program = programs->at(id);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

TEST_F(ProgramContainerTest, programLock) {
	const int waitMs = 100;

	IdType id1 = programs->insert(new Program());
	IdType id2 = programs->insert(new Program());

	auto start = std::chrono::high_resolution_clock::now();

	std::thread thread(getProgramAndWait, programs, id2, waitMs);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

	LockedProgram program1 = programs->at(id1);
	auto finish1 = std::chrono::high_resolution_clock::now();

	LockedProgram program2 = programs->at(id2);
	auto finish2 = std::chrono::high_resolution_clock::now();

	thread.join();

	auto elapsed1 = std::chrono::duration_cast<std::chrono::milliseconds>(finish1 - start);
    EXPECT_LT(elapsed1.count(), waitMs);

	auto elapsed2 = std::chrono::duration_cast<std::chrono::milliseconds>(finish2 - start);
    EXPECT_GE(elapsed2.count(), waitMs);
}
