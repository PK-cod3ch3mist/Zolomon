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

#define STRINGIZING(X) #X
#define STR(X) STRINGIZING(X)
#define FILE_LINE __FILE__ " : line " STR(__LINE__)

namespace zoln {
    // The main namespace that has all the basic test functionalities
    // The basic block is the TestCase class which HAS-A TestResult object

    namespace colours {
        const std::string BASE = "\033[0m";
        const std::string PASS = "\033[32m";
        const std::string FAIL = "\033[31m";
        const std::string TLE = "\033[33m";
        const std::string EXEP = "\033[35m";
    }

    struct case_flags {
        int show_time : 1;
        int show_pass : 1;
    };

    class TestResult {
        bool passed;
        std::string loc;

    public:
        TestResult() {
            passed = true;
        }

        void set_result(bool expression, std::string location) {
            passed = expression;
            loc = location;
        }

        bool test_pass() const { return passed; }

        friend std::ostream& operator <<(std::ostream& out, const TestResult& result);
    };

    class TestCase {
        bool hide;
        TestResult result;
        std::string name;
        std::chrono::duration<double, std::milli> dur;
        case_flags flag;
    public:
        TestCase() {
            hide = false;
            name = "noname";
        }

        TestCase(std::string name, bool expression, bool show_test, std::string location) {
            hide = !show_test;
            this -> name = name;
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            result.set_result(expression, location);
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            dur = t2 - t1;

            // default flag states
            flag.show_time = 0;
            flag.show_pass = 1;
        }

        void set_flags(case_flags flg) {
            flag.show_pass = flag.show_pass | flg.show_pass;
            flag.show_time = flag.show_time | flg.show_time;
        }

        void unset_flags(case_flags flg) {
            flag.show_time = flag.show_time & !flg.show_time;
            flag.show_pass = flag.show_pass & !flg.show_pass;
        }

        bool test_pass() const { return result.test_pass(); }

        friend std::ostream& operator <<(std::ostream& out, const TestCase& test_case);
    };

#define TestCase(name, expr, show) TestCase(name, expr, show, FILE_LINE)

    std::ostream& operator <<(std::ostream& out, const TestResult& result) {
        if (!result.passed) {
            out << "In location " << result.loc << ". ";
        }

        return out;
    }

    std::ostream& operator <<(std::ostream& out, const TestCase& test_case) {
        if (test_case.test_pass() && !test_case.hide && test_case.flag.show_pass) {
            out << "[" << colours::PASS << "PASSED" << colours::BASE << "] : ";
            out << test_case.name << ". ";
            out << test_case.result;
            if (test_case.flag.show_time) {
                out << "Time taken = " << test_case.dur.count() << " ms.";
            }
            out << std::endl;
        }
        else if (!test_case.test_pass() && !test_case.hide) {
            out << "[" << colours::FAIL << "FAILED" << colours::BASE << "] : ";
            out << test_case.name << ". ";
            out << test_case.result;
            if (test_case.flag.show_time) {
                out << "Time taken = " << test_case.dur.count() << " ms.";
            }
            out << std::endl;
        }
        return out;
    }

    class TestCollection {
        std::string _name;
        std::vector<TestCase> _test_cases;
        unsigned long _num_passed;
    public:
        explicit TestCollection(std::string name, std::initializer_list<TestCase> tests) {
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

        void run_collection(std::string args) {
            std::stringstream arguments(args);
            std::string tmp;

            std::set<std::string> flg;

            while (arguments >> tmp) {
                flg.insert(tmp);
            }

            std::cout << "Tests - " << _name << std::endl;
            // if a test case is not hidden, show its results
            for (size_t i = 0; i < _test_cases.size(); i++) {
                if (flg.find("-d") != flg.end()) {
                    _test_cases[i].set_flags({1,0});
                }
                if (flg.find("-p") != flg.end()) {
                    _test_cases[i].set_flags({0,1});
                }
                if (flg.find("-np") != flg.end()) {
                    _test_cases[i].unset_flags({0,1});
                }
                if (flg.find("-nd") != flg.end()) {
                    _test_cases[i].unset_flags({1,0});
                }
                std::cout << _test_cases[i];
            }
            if (_num_passed == _test_cases.size())
                std::cout << colours::PASS << "All Tests Passed\n" << colours::BASE << std::endl;
            else
                std::cout << colours::FAIL << _num_passed << " Passed Out of " << _test_cases.size() << " Tests\n" << colours::BASE << std::endl;
        }
    };
}

#endif