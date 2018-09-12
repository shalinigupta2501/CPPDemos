#include <iostream>

using namespace std;

int main()
{
    int s=5, r=0;
    auto addFive =[s,&r](int a)
    {
        r=(a+s);
    };
    addFive(20);
    cout<<r;
    return 0;
}

