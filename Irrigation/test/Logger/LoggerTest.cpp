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

	ostringstream* o = new ostringstream;

	LOGGER.setOutput(o);
	LOGGER.setLevel(LogLevel::OFF);

	logEntriesOnAllLevels();

	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, logTextError) {

	ostringstream* o = new ostringstream;

	LOGGER.setOutput(o);
	LOGGER.setLevel(LogLevel::ERROR);

	logEntriesOnAllLevels();

	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, logTextWarning) {

	ostringstream* o = new ostringstream;

	LOGGER.setOutput(o);
	LOGGER.setLevel(LogLevel::WARNING);

	logEntriesOnAllLevels();

	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, logTextInfo) {

	ostringstream* o = new ostringstream;

	LOGGER.setOutput(o);
	LOGGER.setLevel(LogLevel::INFO);

	logEntriesOnAllLevels();

	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, logTextDebug) {

	ostringstream* o = new ostringstream;

	LOGGER.setOutput(o);
	LOGGER.setLevel(LogLevel::DEBUG);

	logEntriesOnAllLevels();

	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, logTextTrace) {

	ostringstream* o = new ostringstream;

	LOGGER.setOutput(o);
	LOGGER.setLevel(LogLevel::TRACE);

	logEntriesOnAllLevels();

	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_NE(string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, getLevelText) {
	EXPECT_EQ("OFF", to_string(LogLevel::OFF));
	EXPECT_EQ("ERROR", to_string(LogLevel::ERROR));
	EXPECT_EQ("WARNING", to_string(LogLevel::WARNING));
	EXPECT_EQ("INFO", to_string(LogLevel::INFO));
	EXPECT_EQ("DEBUG", to_string(LogLevel::DEBUG));
	EXPECT_EQ("TRACE", to_string(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableOFF) {
	LOGGER.setLevel(LogLevel::OFF);
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableError) {
	LOGGER.setLevel(LogLevel::ERROR);
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableWarning) {
	LOGGER.setLevel(LogLevel::WARNING);
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableInfo) {
	LOGGER.setLevel(LogLevel::INFO);
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableDebug) {
	LOGGER.setLevel(LogLevel::DEBUG);
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableTrace) {
	LOGGER.setLevel(LogLevel::TRACE);
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::TRACE));
}
