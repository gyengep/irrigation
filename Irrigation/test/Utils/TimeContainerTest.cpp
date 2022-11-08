#include <gmock/gmock.h>
#include <stdexcept>
#include "Utils/TimeContainer.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(TimeContainerTest, empty) {
	try {
		std::vector<std::chrono::milliseconds> times;
		TimeContainer timeContainer(times);
		FAIL();
	} catch (const std::logic_error&) {
	} catch (const std::exception&) {
		FAIL();
	}
}

TEST(TimeContainerTest, oneElement) {
	const std::chrono::milliseconds expected(20);

	std::vector<std::chrono::milliseconds> times{ expected };
	TimeContainer timeContainer(times);

	EXPECT_THAT(timeContainer.getNext(), Eq(expected));
	EXPECT_THAT(timeContainer.getNext(), Eq(expected));
}

TEST(TimeContainerTest, moreElement) {
	const std::chrono::milliseconds expected1(20);
	const std::chrono::milliseconds expected2(30);

	std::vector<std::chrono::milliseconds> times{ expected1, expected2 };
	TimeContainer timeContainer(times);

	EXPECT_THAT(timeContainer.getNext(), Eq(expected1));
	EXPECT_THAT(timeContainer.getNext(), Eq(expected2));
	EXPECT_THAT(timeContainer.getNext(), Eq(expected2));
}

