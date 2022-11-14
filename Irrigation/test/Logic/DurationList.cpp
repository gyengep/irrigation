#include "Logic/DurationList.h"
#include <gmock/gmock.h>

using namespace testing;


TEST(DurationListTest, adjust) {
	const DurationList durations {
		std::chrono::seconds(100),
		std::chrono::seconds(200),
		std::chrono::seconds(300)
	};

	EXPECT_THAT(
		durations.adjust(75),
		ContainerEq(
			DurationList {
				std::chrono::seconds(75),
				std::chrono::seconds(150),
				std::chrono::seconds(225),
			}
		)
	);
}
