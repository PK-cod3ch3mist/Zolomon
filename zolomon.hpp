/**
 * Zolomon 1.0.0
 * Created by Pratyush Kumar
 * Dated: 19 Aug 2021 12:39 PM
 *
 * A Testing System for .CPP files
 * */

#ifndef ZOLOMON_SINGLE_INCLUDE
#define ZOLOMON_SINGLE_INCLUDE

// start header implementation

#define ZOLOMON_VERSION_MAJOR 1
#define ZOLOMON_VERSION_MINOR 0
#define ZOLOMON_VERSION_PATCH 0

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <exception>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <type_traits>
#include <fstream>
#include <functional>

#define STRINGIZING(X) #X
#define STR(X) STRINGIZING(X)
#define FILE_LINE __FILE__ " : line " STR(__LINE__)

namespace zoln
{
// The main namespace that has all the basic test functionalities
// The basic block is the TestCase class which HAS-A TestResult object

namespace colours
{
const std::string BASE = "\033[0m";
const std::string PASSF = "\033[32m";
const std::string PASSB = "\033[42m\033[97m";
const std::string FAILF = "\033[31m";
const std::string FAILB = "\033[41m\033[97m";
const std::string TLE = "\033[33m";
const std::string EXEP = "\033[35m";
const std::string LINF = "\033[90m";
const std::string BOLD = "\033[1m";
}

struct case_flags {
    int show_time : 1;
    int show_pass : 1;
    int fail_notime : 1;
};

template <typename T>
struct identity { typedef T type; };

class TestResult
{
    bool passed;
    std::string loc;

public:
    TestResult()
    {
        passed = true;
    }

    void set_result(bool expression, std::string location)
    {
        passed = expression;
        loc = location;
    }

    bool test_pass() const
    {
        return passed;
    }

    friend std::ostream& operator <<(std::ostream& out, const TestResult& result);
};

template <typename T>
class TestCase
{

    bool hide;
    TestResult result;
    std::string name;
    std::chrono::duration<double, std::milli> dur;
    case_flags flag;
    T acval;
    T exval;

    template<typename Exp_r> friend class TestCollection;

public:
    TestCase()
    {
        hide = false;
        name = "noname";
    }

    TestCase(std::string name, T acval, T exval, bool show_test, std::string location)
    {
        this -> hide = !show_test;
        this -> name = name;
        this -> acval = acval;
        this -> exval = exval;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        result.set_result(acval == exval, location);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        this -> dur = t2 - t1;

        // default flag states
        flag.show_time = 0;
        flag.show_pass = 1;
        flag.fail_notime = 1;
    }

    void set_flags(case_flags flg)
    {
        flag.show_pass = flag.show_pass | flg.show_pass;
        flag.show_time = flag.show_time | flg.show_time;
        flag.fail_notime = flag.fail_notime | flg.fail_notime;
    }

    void unset_flags(case_flags flg)
    {
        flag.show_time = flag.show_time & !flg.show_time;
        flag.show_pass = flag.show_pass & !flg.show_pass;
        flag.fail_notime = flag.fail_notime & !flg.fail_notime;
    }

    bool test_pass() const
    {
        return result.test_pass();
    }
};

#define TestCase(name, expr, eval, show) TestCase(name, expr, eval, show, FILE_LINE)

std::ostream& operator <<(std::ostream& out, const TestResult& result)
{
    out << colours::LINF << result.loc << " -- " << colours::BASE;
    if (result.passed) {
        out << colours::PASSB << "PASSED" << colours::BASE << " ";
    } else {
        out << colours::FAILB << "FAILED" << colours::BASE << " ";
    }
    return out;
}

template <typename Exp_r>
class TestCollection
{
    std::string _name;
    std::vector<TestCase<Exp_r>> _test_cases;
    unsigned long _num_passed;

    template
    <typename T = Exp_r,
     typename std::enable_if<std::is_fundamental<T>::value, int>::type = 0>
    void report_case(TestCase<Exp_r> test_case)
    {
        if (test_case.test_pass() && !test_case.hide && test_case.flag.show_pass) {
            std::cout << test_case.result;
            std::cout << test_case.name << ". ";
            std::cout << "With expression : ";
            std::cout << test_case.acval << " == " << test_case.exval << ". ";
            if (test_case.flag.show_time) {
                std::cout << "Time taken = " << test_case.dur.count() << " ms.";
            }
            std::cout << std::endl;
        } else if (!test_case.test_pass() && !test_case.hide) {
            std::cout << test_case.result;
            std::cout << test_case.name << ". ";
            std::cout << "With expression : ";
            std::cout << test_case.acval << " == " << test_case.exval << ". ";
            if (test_case.flag.show_time && !test_case.flag.fail_notime) {
                std::cout << "Time taken = " << test_case.dur.count() << " ms.";
            }
            std::cout << std::endl;
        }
    }

    template
    <typename T = Exp_r,
     typename std::enable_if<!(std::is_fundamental<T>::value), int>::type = 0>
    void report_case(TestCase<Exp_r> test_case)
    {
        if (test_case.test_pass() && !test_case.hide && test_case.flag.show_pass) {
            std::cout << test_case.result;
            std::cout << test_case.name << ". ";
            if (test_case.flag.show_time) {
                std::cout << "Time taken = " << test_case.dur.count() << " ms.";
            }
            std::cout << std::endl;
        } else if (!test_case.test_pass() && !test_case.hide) {
            std::cout << test_case.result;
            std::cout << test_case.name << ". ";
            if (test_case.flag.show_time && !test_case.flag.fail_notime) {
                std::cout << "Time taken = " << test_case.dur.count() << " ms.";
            }
            std::cout << std::endl;
        }
    }

public:
    explicit TestCollection(std::string name, std::initializer_list<TestCase<Exp_r>> tests)
    {
        _name = name;
        for (auto it = tests.begin(); it != tests.end(); it++) {
            _test_cases.push_back(*it);
        }
        _num_passed = 0;

        for (size_t i = 0; i < _test_cases.size(); i++) {
            if (_test_cases[i].test_pass()) {
                _num_passed++;
            }
        }
    }

    template <typename Func_t, typename T, typename ...Args>
    explicit TestCollection(std::string name, const char * filename, Func_t func, int argn, Args... args) {
        ifstream fin(filename, ios::in);
        string attr;
        getline(fin, attr);
        while (getline(fin, attr)) {
            std::string testName = "";
            size_t index = 0;
            // Take the test name from csv file
            for (; attr[index] != ','; index++) {
                testName += attr[index];
            }
            // Take the function arguments from csv file
            // Use compile-time recursion for this
        }
    } 

    void run_collection()
    {
        std::cout << colours::BOLD << "Tests - " << _name << colours::BASE << std::endl;
        for (size_t i = 0; i < _test_cases.size(); i++) {
            report_case(_test_cases[i]);
        }
        if (_num_passed == _test_cases.size()) {
            std::cout << colours::PASSF << "All Tests Passed\n" << colours::BASE << std::endl;
        } else {
            std::cout << colours::FAILF << _num_passed << " Passed Out of " << _test_cases.size() << " Tests\n" << colours::BASE << std::endl;
        }
    }

    void run_collection(std::string args)
    {
        std::stringstream arguments(args);
        std::string tmp;

        std::set<std::string> flg;

        while (arguments >> tmp) {
            flg.insert(tmp);
        }

        std::cout << colours::BOLD << "Tests - " << _name << colours::BASE << std::endl;
        // if a test case is not hidden, show its results
        for (size_t i = 0; i < _test_cases.size(); i++) {
            if (flg.find("-d") != flg.end()) {
                _test_cases[i].set_flags({1,0,0});
            }
            if (flg.find("-p") != flg.end()) {
                _test_cases[i].set_flags({0,1,0});
            }
            if (flg.find("-np") != flg.end()) {
                _test_cases[i].unset_flags({0,1,0});
            }
            if (flg.find("-nd") != flg.end()) {
                _test_cases[i].unset_flags({1,0,0});
            }
            if (flg.find("-ft") != flg.end()) {
                _test_cases[i].unset_flags({0,0,1});
            }
            if (flg.find("-nft") != flg.end()) {
                _test_cases[i].set_flags({0,0,1});
            }
            report_case(_test_cases[i]);
        }
        if (_num_passed == _test_cases.size()) {
            std::cout << colours::PASSF << "All Tests Passed\n" << colours::BASE << std::endl;
        } else {
            std::cout << colours::FAILF << _num_passed << " Passed Out of " << _test_cases.size() << " Tests\n" << colours::BASE << std::endl;
        }
    }
};
}

#endif
