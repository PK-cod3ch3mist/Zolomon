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
#include <stdexcept>
#include <exception>
#include <vector>
#include <algorithm>

#define STRINGIZING(X) #X
#define STR(X) STRINGIZING(X)
#define FILE_LINE __FILE__ " : line " STR(__LINE__)

namespace zoln {

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
    public:
        TestCase() {
            hide = false;
            name = "noname";
        }

        TestCase(std::string name, bool expression, bool show_test, std::string location) {
            hide = !show_test;
            this -> name = name;
            result.set_result(expression, location);
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
        if (!test_case.hide) {
            if (test_case.test_pass()) out << "[+] : ";
            else out << "[-] : ";
            out << test_case.name << ". ";
            out << test_case.result << std::endl;
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

            // if a test case is not hidden, show its results
            std::cout << "Tests - " << _name << std::endl;
            for (size_t i = 0; i < _test_cases.size(); i++) {
                if (_test_cases[i].test_pass()) {
                    _num_passed++;
                }
                std::cout << _test_cases[i];
            }

            if (_num_passed == _test_cases.size())
                std::cout << "All tests passed.\n" << std::endl;
            else
                std::cout << _num_passed << " passed out of " << _test_cases.size() << " tests.\n" << std::endl;
        }
    };
}

#endif