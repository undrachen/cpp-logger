#include <gtest/gtest.h>

#include <iostream>
#include <regex>
#include <sstream>
#include <streambuf>
#include <string>

#include "cpp_logger/logger.hpp"

struct LogFilteringCase
{
    cpp_logger::LogLevel minimumLevel;
    cpp_logger::LogLevel messageLevel;
};

// Fixture test class
class CTestLogger : public testing::Test
{
public:
    void SetUp()
    {
        mOriginalBuffer = std::cout.rdbuf(mStream.rdbuf());
    }

    void TearDown()
    {
        std::cout.rdbuf(mOriginalBuffer);
    }

    std::string getCapturedStream()
    {
        return mStream.str();
    }

private:
    std::ostringstream mStream;
    std::streambuf *mOriginalBuffer;
};

// Fixture tests

TEST_F(CTestLogger, FormatsDebugMessageCorrectly)
{
    cpp_logger::Logger logger;
    logger.setLevel(cpp_logger::LogLevel::Debug);

    logger.debug("Temperature is normal");

    const std::string output = getCapturedStream();
    const std::regex expectedFormat(R"(\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{4}\] \[DEBUG\] Temperature is normal\n)");

    EXPECT_TRUE(std::regex_match(output, expectedFormat));
}

TEST_F(CTestLogger, FormatsInfoMessageCorrectly)
{
    cpp_logger::Logger logger;
    logger.setLevel(cpp_logger::LogLevel::Debug);

    logger.info("Temperature is increasing");

    const std::string output = getCapturedStream();
    const std::regex expectedFormat(R"(\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{4}\] \[INFO\] Temperature is increasing\n)");

    EXPECT_TRUE(std::regex_match(output, expectedFormat));
}

TEST_F(CTestLogger, FormatsWarningMessageCorrectly)
{
    cpp_logger::Logger logger;
    logger.setLevel(cpp_logger::LogLevel::Debug);

    logger.warning("Temperature is high");

    const std::string output = getCapturedStream();
    const std::regex expectedFormat(R"(\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{4}\] \[WARNING\] Temperature is high\n)");

    EXPECT_TRUE(std::regex_match(output, expectedFormat));
}

TEST_F(CTestLogger, FormatsErrorMessageCorrectly)
{
    cpp_logger::Logger logger;
    logger.setLevel(cpp_logger::LogLevel::Debug);

    logger.error("Temperature is very high");

    const std::string output = getCapturedStream();
    const std::regex expectedFormat(R"(\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{4}\] \[ERROR\] Temperature is very high\n)");

    EXPECT_TRUE(std::regex_match(output, expectedFormat));
}

// Paremeterized test class
class LoggerFilteringTest : public CTestLogger,
                            public testing::WithParamInterface<LogFilteringCase>
{
};

// Parameterized tests

TEST_P(LoggerFilteringTest, FilterLogLevel)
{
    const LogFilteringCase& testCase = GetParam();

    cpp_logger::Logger logger;
    logger.setLevel(testCase.minimumLevel);

    switch (testCase.messageLevel)
    {
        case cpp_logger::LogLevel::Debug:
            logger.debug("Test message");
            break;

        case cpp_logger::LogLevel::Info:
            logger.info("Test message");
            break;

        case cpp_logger::LogLevel::Warning:
            logger.warning("Test message");
            break;

        case cpp_logger::LogLevel::Error:
            logger.error("Test message");
            break;
    }

    if (testCase.messageLevel >= testCase.minimumLevel)
    {
        EXPECT_FALSE(getCapturedStream().empty());
    }
    else
    {
        EXPECT_TRUE(getCapturedStream().empty());
    }
}

INSTANTIATE_TEST_SUITE_P(
    AllLogLevels,
    LoggerFilteringTest,
    testing::Values(
        LogFilteringCase{
            cpp_logger::LogLevel::Debug,
            cpp_logger::LogLevel::Debug,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Debug,
            cpp_logger::LogLevel::Info,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Debug,
            cpp_logger::LogLevel::Warning,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Debug,
            cpp_logger::LogLevel::Error,
        },

        LogFilteringCase{
            cpp_logger::LogLevel::Info,
            cpp_logger::LogLevel::Debug,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Info,
            cpp_logger::LogLevel::Info,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Info,
            cpp_logger::LogLevel::Warning,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Info,
            cpp_logger::LogLevel::Error,
        },

        LogFilteringCase{
            cpp_logger::LogLevel::Warning,
            cpp_logger::LogLevel::Debug,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Warning,
            cpp_logger::LogLevel::Info,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Warning,
            cpp_logger::LogLevel::Warning,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Warning,
            cpp_logger::LogLevel::Error,
        },

        LogFilteringCase{
            cpp_logger::LogLevel::Error,
            cpp_logger::LogLevel::Debug,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Error,
            cpp_logger::LogLevel::Info,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Error,
            cpp_logger::LogLevel::Warning,
        },
        LogFilteringCase{
            cpp_logger::LogLevel::Error,
            cpp_logger::LogLevel::Error,
        }
    )
);
