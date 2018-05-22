#include <gmock/gmock.h>
#include "Logger/Logger.h"

using namespace std;


#define LOG_ENTRY_ERROR 	"log_entry_error"
#define LOG_ENTRY_WARNING 	"log_entry_warning"
#define LOG_ENTRY_INFO 		"log_entry_info"
#define LOG_ENTRY_DEBUG 	"log_entry_debug"
#define LOG_ENTRY_TRACE 	"log_entry_trace"


static void logEntriesOnAllLevels() {
	LOGGER.error(LOG_ENTRY_ERROR);
	LOGGER.warning(LOG_ENTRY_WARNING);
	LOGGER.info(LOG_ENTRY_INFO);
	LOGGER.debug(LOG_ENTRY_DEBUG);
	LOGGER.trace(LOG_ENTRY_TRACE);
}


TEST(LoggerTest, logTextOff) {

	ostringstream o;

	LOGGER.setOutput(o);
	LOGGER.setLevel(Logger::OFF);

	logEntriesOnAllLevels();

	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_ERROR));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_WARNING));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_TRACE));

	LOGGER.setOutput(cout);
}

TEST(LoggerTest, logTextError) {

	ostringstream o;

	LOGGER.setOutput(o);
	LOGGER.setLevel(Logger::ERROR);

	logEntriesOnAllLevels();

	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_ERROR));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_WARNING));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_TRACE));

	LOGGER.setOutput(cout);
}

TEST(LoggerTest, logTextWarning) {

	ostringstream o;

	LOGGER.setOutput(o);
	LOGGER.setLevel(Logger::WARNING);

	logEntriesOnAllLevels();

	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_WARNING));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_TRACE));

	LOGGER.setOutput(cout);
}

TEST(LoggerTest, logTextInfo) {

	ostringstream o;

	LOGGER.setOutput(o);
	LOGGER.setLevel(Logger::INFO);

	logEntriesOnAllLevels();

	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_WARNING));
	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_TRACE));

	LOGGER.setOutput(cout);
}

TEST(LoggerTest, logTextDebug) {

	ostringstream o;

	LOGGER.setOutput(o);
	LOGGER.setLevel(Logger::DEBUG);

	logEntriesOnAllLevels();

	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_WARNING));
	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_INFO));
	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(string::npos, o.str().find(LOG_ENTRY_TRACE));

	LOGGER.setOutput(cout);
}

TEST(LoggerTest, logTextTrace) {

	ostringstream o;

	LOGGER.setOutput(o);
	LOGGER.setLevel(Logger::TRACE);

	logEntriesOnAllLevels();

	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_WARNING));
	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_INFO));
	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_DEBUG));
	EXPECT_NE(string::npos, o.str().find(LOG_ENTRY_TRACE));

	LOGGER.setOutput(cout);
}

TEST(LoggerTest, invalidLevel) {
	Logger::Level level;

	level = static_cast<Logger::Level>(Logger::TRACE + 1);
	EXPECT_THROW(Logger::getLevelText(level), out_of_range);

	level = static_cast<Logger::Level>(-1);
	EXPECT_THROW(Logger::getLevelText(level), out_of_range);
}

TEST(LoggerTest, getLevelText) {
	EXPECT_STREQ("OFF", Logger::getLevelText(Logger::OFF));
	EXPECT_STREQ("ERROR", Logger::getLevelText(Logger::ERROR));
	EXPECT_STREQ("WARNING", Logger::getLevelText(Logger::WARNING));
	EXPECT_STREQ("INFO", Logger::getLevelText(Logger::INFO));
	EXPECT_STREQ("DEBUG", Logger::getLevelText(Logger::DEBUG));
	EXPECT_STREQ("TRACE", Logger::getLevelText(Logger::TRACE));
}

TEST(LoggerTest, isLoggableOFF) {
	LOGGER.setLevel(Logger::OFF);
	EXPECT_FALSE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST(LoggerTest, isLoggableError) {
	LOGGER.setLevel(Logger::ERROR);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST(LoggerTest, isLoggableWarning) {
	LOGGER.setLevel(Logger::WARNING);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST(LoggerTest, isLoggableInfo) {
	LOGGER.setLevel(Logger::INFO);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST(LoggerTest, isLoggableDebug) {
	LOGGER.setLevel(Logger::DEBUG);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST(LoggerTest, isLoggableTrace) {
	LOGGER.setLevel(Logger::TRACE);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::TRACE));
}
