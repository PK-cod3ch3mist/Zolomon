# Zolomon
Another single header testing library for C++.

## Contents

- [About](#about)
- How to Use
    - [Installation](#installation-and-set-up)
    - [Quick Start](#a-few-simple-examples)
    - [Docs](#documentation)
- License
- Contributors

## About
Zolomon is  a single header testing library for C++, that reports file and line number in case of error, and can be used to create unit tests

The name is inspired from the DC character Zoom, who creates tragic situations for heroes to make them better. Similar to what you can do to your programs using the library.

## How to Use
### Installation and Set Up
Simply download the [header file](https://raw.githubusercontent.com/PK-cod3ch3mist/Zolomon/main/zolomon.hpp?token=ANHLDA6AV5SHVJRQVTHP4NTBFIGVY) and place it along with your project, and use as you would use any other C++ library. Use the namespace zoln when declaring test cases.

### A Few Simple Examples
Below is a simple file that demonstrates the use of Zolomon library
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
	    zoln::TestCase("Test Case 1", v[0] + v[1] == 2, false),
	    zoln::TestCase("Test Case 2", v[0] + v[1] == 3, true)
	});
	return 0;
}
```
Compiling the above code and running the executable would give the following output.
```shell
Tests - Sample Test 1
[+] : Test Case 1. 
[-] : Test Case 2. In location testzolomon.cpp : line 9. 
1 passed out of 2 tests.

Tests - Sample Test 2
[+] : Test Case 2. 
1 passed out of 2 tests.
```

### Documentation
Documentation is available in .pdf and .md format. Click on the links to view it.