#include "common.h"
#include "LoggerTest.h"


void LoggerTest::SetUp() {
	levelSave = LOGGER.getLevel();
}

void LoggerTest::TearDown() {
	LOGGER.setLevel(levelSave);
}


TEST_F(LoggerTest, getLevelText) {
	EXPECT_STREQ("OFF", Logger::getLevelText(Logger::OFF));
	EXPECT_STREQ("ERROR", Logger::getLevelText(Logger::ERROR));
	EXPECT_STREQ("WARNING", Logger::getLevelText(Logger::WARNING));
	EXPECT_STREQ("INFO", Logger::getLevelText(Logger::INFO));
	EXPECT_STREQ("DEBUG", Logger::getLevelText(Logger::DEBUG));
	EXPECT_STREQ("TRACE", Logger::getLevelText(Logger::TRACE));
}

TEST_F(LoggerTest, isLoggableOFF) {
	LOGGER.setLevel(Logger::OFF);
	EXPECT_FALSE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST_F(LoggerTest, isLoggableError) {
	LOGGER.setLevel(Logger::ERROR);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST_F(LoggerTest, isLoggableWarning) {
	LOGGER.setLevel(Logger::WARNING);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST_F(LoggerTest, isLoggableInfo) {
	LOGGER.setLevel(Logger::INFO);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST_F(LoggerTest, isLoggableDebug) {
	LOGGER.setLevel(Logger::DEBUG);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST_F(LoggerTest, isLoggableTrace) {
	LOGGER.setLevel(Logger::TRACE);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::TRACE));
}
