#include "Views/RestView/PathTemplate.h"
#include <gmock/gmock.h>
#include <stdexcept>

using namespace std;
using namespace testing;


TEST(SplitPathTest, validOrNot) {
	Path path;

	EXPECT_THROW(PathTemplate::split("", path), invalid_argument);
	EXPECT_NO_THROW(PathTemplate::split("/", path));
	EXPECT_THROW(PathTemplate::split("//", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("123", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("123/", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("123//", path), invalid_argument);
	EXPECT_NO_THROW(PathTemplate::split("/123", path));
	EXPECT_THROW(PathTemplate::split("/123/", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("/123//", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("//123", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("//123/", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("//123//", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("123/abcef", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("123/abcef/", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("123/abcef//", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("123//abcef", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("123//abcef/", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("123//abcef//", path), invalid_argument);
	EXPECT_NO_THROW(PathTemplate::split("/123/abcef", path));
	EXPECT_THROW(PathTemplate::split("/123/abcef/", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("/123/abcef//", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("/123//abcef", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("/123//abcef/", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("/123//abcef//", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("//123/abcef", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("//123/abcef/", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("//123/abcef//", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("//123//abcef", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("//123//abcef/", path), invalid_argument);
	EXPECT_THROW(PathTemplate::split("//123//abcef//", path), invalid_argument);
}

TEST(SplitPathTest, vaildPath1) {
	Path path;
	PathTemplate::split("/", path);
	EXPECT_THAT(path, Eq(Path()));
}

TEST(SplitPathTest, vaildPath2) {
	Path path;
	PathTemplate::split("/abcde", path);
	EXPECT_THAT(path, Eq(Path({"abcde"})));
}

TEST(SplitPathTest, vaildPath3) {
	Path path;
	PathTemplate::split("/XY/qwert", path);
	EXPECT_THAT(path, Eq(Path({"XY", "qwert"})));
}

