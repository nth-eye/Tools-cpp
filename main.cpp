#include <iostream>
#include "tools.h"

auto fact(int a)
{
    int res = 1;
    for (int i = 1; i < a; ++i)
        res *= i;
    return a;
};

int main()
{
    std::cout << measure_time<1000000>(fact, 20) << " ns" <<std::endl;
    std::cout << measure_time<1000000>(fact, 10) << " ns" <<std::endl;
}