#include "Views/RestView/PathTemplate.h"
#include "WebServer/KeyValue.h"
#include <gmock/gmock.h>
#include <stdexcept>

using namespace testing;


TEST(PathTemplateTest, invalidPathTemplate) {
	EXPECT_THROW(PathTemplate("/abc/{}/123"), std::invalid_argument);
	EXPECT_THROW(PathTemplate("{variable}"), std::invalid_argument);
}

TEST(PathTemplateTest, pathTemplateNotEquals) {
	PathTemplate pathTemplate("/abc/{variable1}/123");
	KeyValue parameters;

	EXPECT_FALSE(pathTemplate.evaluate(Path{"abc"}, parameters));
	EXPECT_FALSE(pathTemplate.evaluate(Path{"abc", "value1"}, parameters));
	EXPECT_FALSE(pathTemplate.evaluate(Path{"abc", "123"}, parameters));
	EXPECT_FALSE(pathTemplate.evaluate(Path{"abc", "123", "987654"}, parameters));
	EXPECT_FALSE(pathTemplate.evaluate(Path{"abc", "value1", "123", "xyz"}, parameters));
}

TEST(PathTemplateTest, pathTemplateEquals1) {
	PathTemplate pathTemplate("/abc/{variable1}/123");
	KeyValue parameters;

	EXPECT_TRUE(pathTemplate.evaluate(Path{"abc", "value1", "123"}, parameters));
	EXPECT_EQ(KeyValue({{"variable1", "value1"}}), parameters);
}

TEST(PathTemplateTest, pathTemplateEquals2) {
	PathTemplate pathTemplate("/abc/{variable1}/123/");
	KeyValue parameters;

	EXPECT_TRUE(pathTemplate.evaluate(Path{"abc", "value1", "123", ""}, parameters));
	EXPECT_EQ(KeyValue({{"variable1", "value1"}}), parameters);
}

TEST(PathTemplateTest, pathTemplateEquals3) {
	PathTemplate pathTemplate("/path1/{variable1}/resource1/{variable2}/resource2");
	KeyValue parameters;

	EXPECT_TRUE(pathTemplate.evaluate(Path{"path1", "1234", "resource1", "value2", "resource2"}, parameters));
	EXPECT_EQ(KeyValue({{"variable1", "1234"}, {"variable2", "value2"}}), parameters);
}

TEST(PathTemplateTest, pathTemplateEquals4) {
	PathTemplate pathTemplate("/{variable1}/resource1/{variable2}");
	KeyValue parameters;

	EXPECT_TRUE(pathTemplate.evaluate(Path{"1234", "resource1", "value2"}, parameters));
	EXPECT_EQ(KeyValue({{"variable1", "1234"}, {"variable2", "value2"}}), parameters);
}
