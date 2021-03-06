//how smart pointer works
#include<iostream>
using namespace std;

class SmartPtr
{
int *ptr; // Actual pointer
public:
// Constructor: Refer https://www.geeksforgeeks.org/g-fact-93/
// for use of explicit keyword
explicit SmartPtr(int *p = NULL) { ptr = p; }

// Destructor
~SmartPtr() { delete(ptr); }

// Overloading dereferencing operator
int &operator *() { return *ptr; }
};

int main()
{
	SmartPtr ptr(new int());
	//int *p = new int;

//    int x;
//    x=2;
//    int a=10;
//    int b(20);
//

	*ptr = 20;
	cout << *ptr;

	// We don't need to call delete ptr: when the object
	// ptr goes out of scope, destructor for it is automatically
	// called and destructor does delete ptr.

	return 0;
}
