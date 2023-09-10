#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#ifndef SIMPLE_UNIT_TEST_HPP
#define SIMPLE_UNIT_TEST_HPP

class TestFramework
{
    using TestCaseFunc = std::function<void()>;

    std::vector<std::pair<std::string, TestCaseFunc>> m_testCases{};
    std::vector<std::pair<std::string, std::pair<uint64_t, uint64_t>>> m_casesAssertions{};
    uint64_t m_passedAssertions{};
    uint64_t m_failedAssertions{};
    bool m_isPassedAssertion{};

  public:
    static TestFramework &getInstance()
    {
        static TestFramework instance;
        return instance;
    };

    void updatePassedAssertions()
    {
        ++m_passedAssertions;
    }

    void updateFailedAssertions()
    {
        m_isPassedAssertion = false;
        ++m_failedAssertions;
    }

    void registerTestCase(const std::string &t_testDescription, const TestCaseFunc &t_testFunc)
    {
        m_casesAssertions.emplace_back(t_testDescription, std::make_pair(m_passedAssertions, m_failedAssertions));
        m_testCases.emplace_back(t_testDescription, t_testFunc);
    }

    void runTests()
    {
        std::vector<std::pair<std::string, bool>> testResults{};
        uint64_t totalCases = m_testCases.size();
        uint64_t failedCases{};
        uint64_t totalPassedAssertions{};
        uint64_t totalFailedAssertions{};

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        std::cout << "\nSimple Unit Testing\n\n";
        std::cout << "====================================================================\n\n";

        for (const auto &testCase : m_testCases)
        {
            try
            {
                m_passedAssertions = 0;
                m_failedAssertions = 0;
                m_isPassedAssertion = true;

                testCase.second();

                if (m_isPassedAssertion)
                {
                    testResults.emplace_back(testCase.first, true);
                }
                else
                {
                    testResults.emplace_back(testCase.first, false);
                    ++failedCases;
                }

                totalPassedAssertions += m_passedAssertions;
                totalFailedAssertions += m_failedAssertions;
            }
            catch (const std::exception &e)
            {
                testResults.emplace_back(testCase.first, false);
                ++failedCases;
                ++totalFailedAssertions;

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                std::cout << "Unhandled exception: " << e.what() << "\n" << std::flush;
            }
        }

        for (const auto &testResult : testResults)
        {
            if (testResult.second)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                std::cout << "-- [  PASSED  ] -- " << testResult.first << "\n";
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                std::cout << "-- [  FAILED  ] -- " << testResult.first << "\n";
            }
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        std::cout << "\n====================================================================\n";
        std::cout << "Total tests " << totalCases << " : " << totalCases - failedCases << " passed | " << failedCases
                  << " failed\n";
        std::cout << "Total assertions " << totalFailedAssertions + totalPassedAssertions << " : "
                  << totalPassedAssertions << " : passed | " << totalFailedAssertions << " failed\n\n"
                  << std::flush;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    }
};

class TestUnit
{
    using TestCaseFunc = std::function<void()>;

  public:
    TestUnit(const std::string &t_testDescription, const TestCaseFunc &t_testFunc)
    {
        TestFramework::getInstance().registerTestCase(t_testDescription, t_testFunc);
    };
};

#define INTERNAL_UNIQUE_NAME_LINE2(t_name, t_line) t_name##t_line
#define INTERNAL_UNIQUE_NAME_LINE(t_name, t_line) INTERNAL_UNIQUE_NAME_LINE2(t_name, t_line)
#define INTERNAL_UNIQUE_NAME(t_name) INTERNAL_UNIQUE_NAME_LINE(t_name, __COUNTER__)

#define INTERNAL_TEST_CASE2(t_testName, t_testDescription)                                                             \
    void t_testName();                                                                                                 \
    TestUnit INTERNAL_UNIQUE_NAME(autoRegister)(t_testDescription, t_testName);                                        \
    void t_testName()
#define INTERNAL_TEST_CASE(...) INTERNAL_TEST_CASE2(INTERNAL_UNIQUE_NAME(t_testName), __VA_ARGS__)

#define TEST_CASE(...) INTERNAL_TEST_CASE(__VA_ARGS__)

#define ASSERT(condition)                                                                                              \
    if ((condition))                                                                                                   \
        TestFramework::getInstance().updatePassedAssertions();                                                         \
    else                                                                                                               \
        TestFramework::getInstance().updateFailedAssertions();

int main(void)
{
    TestFramework::getInstance().runTests();

    return 0;
}

#endif