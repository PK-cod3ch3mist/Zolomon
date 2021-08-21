// The simple example to demonstrate the use of Zolomon header file

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