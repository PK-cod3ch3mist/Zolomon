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
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <exception>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <type_traits>

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

enum report_format {
    // report_junitXML,
    report_HTML,
    report_console
}result_show;
struct case_flags {
    int show_time : 1;
    int show_pass : 1;
    int fail_notime : 1;
}flags;

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
    friend std::ofstream& operator <<(std::ofstream& fout, const TestResult& result);
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

std::ofstream& operator <<(std::ofstream& fout, const TestResult& result) {
    fout << "<td>" << result.loc << "</td>";
    std::string message = ((result.passed) ? "<td bgcolor=LightGreen>PASSED</td>" : "<td bgcolor=LightCoral>FAILED</td>");
    fout << message;
    return fout;
}

void parse_args(int argc, char* argv[]) {
    flags.show_time = 1;
    flags.show_pass = 0;
    flags.fail_notime = 1;
    result_show = report_console;
    try {
        for (int i = 1; i < argc; i++) {
            std::string argument = argv[i];
            if (argument == "-d") {
                flags.show_time = 1;
            } else if (argument == "-p") {
                flags.show_pass = 1;
            } else if (argument == "-np") {
                flags.show_pass = 0;
            } else if (argument == "-nd") {
                flags.show_time = 0;
            } else if (argument == "-ft") {
                flags.fail_notime = 0;
            } else if (argument == "-nft") {
                flags.fail_notime = 1;
            } else if (argument == "-html") {
                result_show = report_HTML;
            } else {
                throw (argument);
            }
        }
    }
    catch (std::string arg) {
        std::cerr << "Invalid argument provided: " << arg << std::endl;
        exit(1);
    }

    if (result_show == report_HTML) {
        std::ofstream fout("testlog.html", std::ios::trunc);
        time_t now = time(0);
        tm* gmtm = gmtime(&now);
        char* dt = asctime(gmtm);
        fout << "<!DOCTYPE html><html lang=\"en\"><head><style>body{font-family: sans-serif;} table, th, td { border:1px solid black; }</style><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Test Log</title></head><body>";
        fout << "<h1 style=\"text-align:center;\">Test Run</h1><h2 style=\"text-align:center;\">" << dt << " UTC</h2>";
        dt = ctime(&now);
        fout << "<h2 style=\"text-align:center;\">" << dt << " Local time</h2>";
        fout.close();
    }
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
    void report_case(TestCase<Exp_r> test_case, report_format rpt)
    {
        if ((test_case.test_pass() && !test_case.flag.show_pass) || test_case.hide) {
            return;
        }
        if (rpt == report_console) {
            std::cout << test_case.result;
            std::cout << test_case.name << ". ";
            std::cout << "With expression : ";
            std::cout << test_case.acval << " == " << test_case.exval << ". ";
            if (test_case.flag.show_time && (test_case.test_pass() || !test_case.flag.fail_notime)) {
                std::cout << "Time taken = " << test_case.dur.count() << " ms.";
            }
            std::cout << std::endl;
        } else if (rpt == report_HTML) {
            std::ofstream fout("testlog.html", std::ios::app);
            fout << "<tr>";
            fout << "<td><strong>" << test_case.name << "</strong></td>";
            fout << test_case.result;
            fout << "<td>" << test_case.acval << " == " << test_case.exval << "</td>";
            if (test_case.flag.show_time && (test_case.test_pass() || !test_case.flag.fail_notime)) {
                fout << "<td>" << test_case.dur.count() << " ms</td>";
            } else {
                fout << "<td style=\"color:grey\"> Duration not required </td>";
            }
            fout << "</tr>";
            fout.close();
        }
    }

    template
    <typename T = Exp_r,
     typename std::enable_if<!(std::is_fundamental<T>::value), int>::type = 0>
    void report_case(TestCase<Exp_r> test_case, report_format rpt)
    {
        if ((test_case.test_pass() && !test_case.flag.show_pass) || test_case.hide) {
            return;
        }
        if (rpt == report_console) {
            std::cout << test_case.result;
            std::cout << test_case.name << ". ";
            if (test_case.flag.show_time && (test_case.test_pass() || !test_case.flag.fail_notime)) {
                std::cout << "Time taken = " << test_case.dur.count() << " ms.";
            }
            std::cout << std::endl;
        } else if (rpt == report_HTML) {
            std::ofstream fout("testlog.html", std::ios::app);
            // fout << "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Test Log</title></head><body>"
            fout << "<tr>";
            fout << "<td><strong>" << test_case.name << "</strong></td>";
            fout << test_case.result;
            fout << "<td style=\"color:grey\">" << "Cannot be shown" << "</td>";
            if (test_case.flag.show_time && (test_case.test_pass() || !test_case.flag.fail_notime)) {
                fout << "<td>" << test_case.dur.count() << " ms</td>";
            } else {
                fout << "<td style=\"color:grey\"> Duration not required </td>";
            }
            fout << "</tr>";
            fout.close();
        }
    }
public:
    /// @brief Test collection constructor that takes in an initializer list of test cases to run
    /// @param name Name of the test collection
    /// @param tests Initializer list containing test cases instances
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

    /// @brief Runs the test collection with the command line arguments
    void run_collection()
    {
        if (result_show == report_console) {
            std::cout << colours::BOLD << "Tests - " << _name << colours::BASE << std::endl;
        } else if (result_show == report_HTML) {
            std::ofstream fout("testlog.html", std::ios::app);
            fout << "<h3 style=\"text-align:center;\">Tests - " << _name << "</h3>";
            fout << "<table style=\"width:100%;\"><tr><th>Name</th><th>Location</th><th>Status</th><th>Expression</th><th>Duration</th></tr>";
            fout.close();
        }
        for (size_t i = 0; i < _test_cases.size(); i++) {
            _test_cases[i].unset_flags({1, 1, 1});
            _test_cases[i].set_flags(flags);
            report_case(_test_cases[i], result_show);
        }
        if (_num_passed == _test_cases.size()) {
            if (result_show == report_console)
                std::cout << colours::PASSF << "All Tests Passed\n" << colours::BASE << std::endl;
            else if (result_show == report_HTML) {
                std::ofstream fout("testlog.html", std::ios::app);
                fout << "</table><p style=\"color:green\"><strong>All Tests Passed</strong></p>";
                fout.close();
            }
        } else {
            if (result_show == report_console)
                std::cout << colours::FAILF << _num_passed << " Passed Out of " << _test_cases.size() << " Tests\n" << colours::BASE << std::endl;
            else if (result_show == report_HTML) {
                std::ofstream fout("testlog.html", std::ios::app);
                fout << "</table><p style=\"color:red\"><strong>" << _num_passed << " Passed out of " << _test_cases.size() << " tests</strong></p>";
                fout.close();
            }
        }
    }

    /// @brief Runs the test collection with provided arguments
    /// @param args String with all arguments in same format as the command line arguments
    void run_collection(std::string args)
    {
        std::stringstream arguments(args);
        std::string tmp;

        std::set<std::string> flg;

        while (arguments >> tmp) {
            flg.insert(tmp);
        }

        if (result_show == report_console) {
            std::cout << colours::BOLD << "Tests - " << _name << colours::BASE << std::endl;
        } else if (result_show == report_HTML) {
            std::ofstream fout("testlog.html", std::ios::app);
            fout << "<h3 style=\"text-align:center;\">Tests - " << _name << "</h3>";
            fout << "<table style=\"width:100%;\"><tr><th>Name</th><th>Location</th><th>Status</th><th>Expression</th><th>Duration</th></tr>";
            fout.close();
        }
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
            report_case(_test_cases[i], result_show);
        }
        if (_num_passed == _test_cases.size()) {
            if (result_show == report_console)
                std::cout << colours::PASSF << "All Tests Passed\n" << colours::BASE << std::endl;
            else if (result_show == report_HTML) {
                std::ofstream fout("testlog.html", std::ios::app);
                fout << "</table><p style=\"color:green\"><strong>All Tests Passed</strong></p>";
                fout.close();
            }
        } else {
            if (result_show == report_console)
                std::cout << colours::FAILF << _num_passed << " Passed Out of " << _test_cases.size() << " Tests\n" << colours::BASE << std::endl;
            else if (result_show == report_HTML) {
                std::ofstream fout("testlog.html", std::ios::app);
                fout << "</table><p style=\"color:red\"><strong>" << _num_passed << " Passed out of " << _test_cases.size() << " tests</strong></p>";
                fout.close();
            }
        }
    }
};

void end_report() {
    if (result_show == report_console) {
        std::cout << "=========== End of Test Run ===========" << std::endl;
    } else if (result_show == report_HTML) {
        std::ofstream fout("testlog.html", std::ios::app);
        fout << "<p style=\"text-align:center;\">End of test report</p></body></html>";
        fout.close();
    }
}
}

#endif
