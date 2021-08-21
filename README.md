# Zolomon
Another single header testing library for C++.

## Contents

- [About](#about)
- How to Use
    - [Installation](#installation-and-set-up)
    - [Quick Start](#a-simple-example)
- License
- Contributors

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

int main() {
    std::vector<int> v = {1, 2, 0, 3, 0, 1, 4, 2, 1};
    zoln::TestCollection Test1("Sample Test 1" , {
        zoln::TestCase("Test Case 1", v[0] == 1, true),
        zoln::TestCase("Test Case 2", v[2] == 1, true)
    });
    zoln::TestCollection Test2("Sample Test 2", {
        zoln::TestCase("Test Case 1", v[0] + v[1] == 2, true),
        zoln::TestCase("Test Case 2", v[0] + v[1] == 3, true)
    });
    Test1.run_collection("-d -p");
    Test2.run_collection("-nd -np");
	return 0;
}
```
`zoln::TestCase` is the basic block of the library which has the parameters: `name`, `expression`, and `show`/`hide` respectively. The `name` is the name of the test case, `expression` is what you want to check and `show` is a boolean which is true if you want to show and false if you want to hide the result of that test.

**Note that hidden test cases are also run, just not shown.**

Compiling the above code and running the executable would give the following output.
![Simple Eg Output]()