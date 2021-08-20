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
#include <list>
#include <algorithm>

namespace zoln {

    template <typename T>
    class TestResult {
        bool passed;
        std::string file;
        std::string line;

    public:
        TestResult() {
            passed = true;
            file = __FILE__;
            line = __LINE__;
        }

        void set_result(T expression, T expected_val) {
            if (expression != expected_val)
                passed = false;
        }

        bool test_pass() return passed;

        friend std::ostream& operator <<(std::ostream& out, const TestResult& result);
    };

    template <typename X>
    class TestCase {
        bool hide;
        TestResult result;
        std::string name;
    public:
        TestCase() {
            hide = false;
            name = "noname";
        }

        TestCase(std::string name, X expression, X expected_val, bool show_test) {
            hide = !show_test;
            this.name = name;
            result.set_result(expression, expected_val);
        }

        bool test_pass() return result.test_pass();

        friend std::ostream& operator <<(std::ostream& out, const TestCase& case);
    };

    std::ostream& operator <<(std::ostream& out, const TestResult& result) {
        if (result.passed) out << "[+] : ";
        else {
            out << "[-] : In file " << result.file << " and line " << result.line << std::endl;
        }

        return out;
    }

    std::ostream& operator <<(std::ostream& out, const TestCase& case) {
        if (!hide) {
            out << case.result;
            out << "Test - " << case.name << std::endl;
        } else {
            out << "";
        }

        return out;
    }

    class TestCollection {
        std::string name;
        std::list<TestCase> cases;
        unsigned long num_passed;
    public:
        explicit TestCollection(std::string nm, std::initializer_list<TestCase> cases) {
            this.name = nm;
            for (size_t i = 0; i < cases.size(); i++) {
                this.cases.push_back(cases[i]);
            }
            num_passed = 0;

            // if a test case is not hidden, show its results
            cout << "Tests - " << this.name << std::endl;
            for (size_t i = 0; i < this.cases.size(); i++) {
                if (this.cases[i].test_pass()) {
                    num_passed++;
                }
                cout << this.cases[i];
            }

            cout << num_passed << " passed out of " << this.cases.size() << " tests." << std::endl;
        }
    };
}

#endif