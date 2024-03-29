<p align="center"><a href="https://github.com/PK-cod3ch3mist/Zolomon"><img src="./zolomon.png" alt="Zolomon" width="450px"></a></p>
Another single header testing library for C++.

![Lines of code](https://img.shields.io/tokei/lines/github/PK-cod3ch3mist/Zolomon)
![GitHub issues](https://img.shields.io/github/issues/PK-cod3ch3mist/Zolomon)
![GitHub Repo stars](https://img.shields.io/github/stars/PK-cod3ch3mist/Zolomon)
![GitHub last commit](https://img.shields.io/github/last-commit/PK-cod3ch3mist/Zolomon)


## Contents

- [About](#about)
- How to Use
    - [Installation](#installation-and-set-up)
    - [Quick Start](#a-simple-example)

## About
Zolomon is  a single header testing library for C++, that reports file and line number in case of error, and can be used to create unit tests

The name is inspired from the DC character Zoom, who creates tragic situations for heroes to make them better. Similar to what you can do to your programs using the library.

## How to Use
### Installation and Set Up
Simply download the [header file](https://raw.githubusercontent.com/PK-cod3ch3mist/Zolomon/main/zolomon.hpp?token=ANHLDA6AV5SHVJRQVTHP4NTBFIGVY) and place it along with your project, and use as you would use any other C++ library. Use the namespace zoln when declaring test cases.

### A Simple Example
Below is a simple file that demonstrates the use of Zolomon library.
```c++
#include <iostream>
#include <vector>
#include "zolomon.hpp"

int main(int argc, char* argv[])
{
    std::vector<int> v = {1, 2, 0, 3, 0, 1, 4, 2, 1};
    zoln::parse_args(argc, argv);
    zoln::TestCollection Test1("Sample Test 1", {
        zoln::TestCase("Test Case 1", v[0], 1, true),
        zoln::TestCase("Test Case 2", v[2], 0, true)
    });
    zoln::TestCollection Test2("Sample Test 2", {
        zoln::TestCase("Test Case 1", v[0] + v[1], 2, true),
        zoln::TestCase("Test Case 2", v[0] + v[1], 3, true)
    });
    Test1.run_collection();
    Test2.run_collection();
    zoln::end_report();
    return 0;
}
```
`zoln::TestCase` is the basic block of the library which has the parameters: `name`, `actual value`, `expected value` and `show`/`hide` respectively. The `name` is the name of the test case, `actual value` and `expected value` are what you want to check and `show` is a boolean which is true if you want to show and false if you want to hide the result of that test.

**Note that hidden test cases are also run, just not shown.**

Compiling the above code and running the executable (with argument `-p`) would give the following output.

![Simple Eg Output](https://raw.githubusercontent.com/PK-cod3ch3mist/Zolomon/main/simple.png)

Running with arguments `-p`, `-html` gives the output shown in [testlog.html](./testlog.html) file (available for [preview](https://htmlpreview.github.io/?https://github.com/PK-cod3ch3mist/Zolomon/blob/main/testlog.html))

**Note that arguments can also be passed in the `run_collection()` function, where they override the console arguments for the particular `TestCollection`**

The strings passed while running are:
- \-d : show run time of test cases
- \-p : show passed test cases
- \-nd : do not show run time of test cases
- \-np : do not show passed test cases
- \-ft : show time for failed test
- \-nft : do not show time for failed tests

**The order of the arguments when running a test collection does not matter.**

### A More Sophisticated Example

The following program, when given an array of integers, returns a new array such that each number at index i of the new array is the product of all the numbers in original array except the one at index i. Zolomon is used to test this program.

**Note that in case of compound types the library does not show actual and expected variables.** This is being worked upon and will be available in the future.

```c++
#include <iostream>
#include <vector>
#include "zolomon.hpp"

std::vector<int> prodVectorNoDivision(std::vector<int> &nums)
{
    std::vector<long int> prefix(nums.size());
    std::vector<long int> suffix(nums.size());
    std::vector<int> result(nums.size());

    prefix[0] = 1;
    suffix.back() = 1;

    for (int i = 1; i < nums.size(); i++) {
        prefix[i] = nums[i - 1] * prefix[i - 1];
    }
    for (int i = nums.size() - 2; i >= 0; i--) {
        suffix[i] = nums[i + 1] * suffix[i + 1];
    }
    for (int i = 0; i < result.size(); i++) {
        result[i] = prefix[i] * suffix[i];
    }
    return result;
}

int main(int argc, char* argv[])
{
    std::vector<int> nums1 = {1, 2, 2, 3, 1};
    std::vector<int> nums2 = {0, 1, 2, 1, 2};

    zoln::parse_args(argc, argv);
    zoln::TestCollection Test1("Sample Test", {
        zoln::TestCase("Basic Test", prodVectorNoDivision(nums1), std::vector<int>({12, 6, 6, 4, 12}), true),
        zoln::TestCase("Handling Zeroes", prodVectorNoDivision(nums2), std::vector<int>({4, 0, 0, 0, 0}), true)
    });

    Test1.run_collection("-p -d");
    zoln::end_report();
    return 0;
}
```

This returns the following output.

![Sophisticated Example](https://raw.githubusercontent.com/PK-cod3ch3mist/Zolomon/main/sophisticated.png)
