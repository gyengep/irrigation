#include "common.h"
#include "Utils/UniqueID.h"

#include <thread>


void getInstances(size_t count, std::vector<IdType>* ids) {
	ids->resize(count);
	for(size_t i = 0; i < count; ++i) {
		ids->at(i) = UniqueID::getInstance().getNextId();
	}
}


TEST(UniqueID, concurent) {
	const size_t threadCount = 5;
	const size_t getInstanceCount = 10000;

	std::vector<IdType> idVectors[threadCount];

    std::thread getInstancesThread[] = {
    		{ std::thread(getInstances, getInstanceCount, &idVectors[0]) },
    		{ std::thread(getInstances, getInstanceCount, &idVectors[1]) },
    		{ std::thread(getInstances, getInstanceCount, &idVectors[2]) },
    		{ std::thread(getInstances, getInstanceCount, &idVectors[3]) },
    		{ std::thread(getInstances, getInstanceCount, &idVectors[4]) }
    };

    for (size_t i = 0; i < threadCount; ++i) {
        getInstancesThread[i].join();
    }

    std::set<IdType> idSet;
    for (size_t i = 0; i < threadCount; ++i) {
    	for (size_t t = 0; t < idVectors[i].size(); ++t) {
    		idSet.insert(idVectors[i].at(t));
    	}
    }

    EXPECT_EQ(threadCount * getInstanceCount, idSet.size());
}

