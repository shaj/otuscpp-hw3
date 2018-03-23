



#include <iostream>
#include <vector>
#include <map>

#include "allocator.h"
//#include "mylist.h"

long double fact(int N)
{
    if(N < 0) // если пользователь ввел отрицательное число
        return 0; // возвращаем ноль
    if (N == 0) // если пользователь ввел ноль,
        return 1; // возвращаем факториал от нуля - не удивляетесь, но это 1 =)
    else // Во всех остальных случаях
        return N * fact(N - 1); // делаем рекурсию.
}






int main (int, char *[])
{
/*	
	auto v = std::vector<int, logging_allocator<int>>{};

	v.reserve(5);

	for(size_t i=0; i<6; ++i)
	{
		v.emplace_back(i);
		std::cout << std::endl;
	}

	for(auto i: v)
	{
		std::cout << i << std::endl;
	}


*/


	auto m1 = std::map<int, long double>{};
	for(size_t i=0; i<10; i++)
	{
		m1[i] = fact(i);
	}
	for(auto it: m1)
	{
		std::cout << it.first << " " << it.second << std::endl;
	}

	auto m2 = std::map<int, long double, std::less<int>, logging_allocator<std::pair<const int, long double>>> {};
	for(size_t i=0; i<10; ++i)
	{
		m2[i] = fact(i);
	}
	for(auto it: m2)
	{
		std::cout << it.first << " " << it.second << std::endl;
	}

/*
	auto m3 = mylist<int>{};
	for(size_t i=0; i<10; i++)
	{
		m3.append(i);
	}
	for(auto it: m3)
	{
		std::cout << *it << std::endl;
	}

*/
	return 0;
}

