#include <iostream>
#include "tools.h"

auto fact_count(int a)
{
    int res = 1;
    for (int i = 1; i < a; ++i)
        res *= i;
    for (int i = 0; i < res; ++i)
        --a;
    return a;
};

int main()
{
    std::cout << tool::measure_time<100>(fact_count, 11) << " ns" <<std::endl;
    std::cout << tool::measure_time<100, tool::microseconds>(fact_count, 11) << " us" <<std::endl;
    std::cout << tool::measure_time<100, tool::milliseconds>(fact_count, 11) << " ms" <<std::endl;
}