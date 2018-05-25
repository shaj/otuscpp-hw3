


#include <ios>
#include <iostream>
#include <vector>
#include <map>

#include "log.h"

#include "newdelete.h"
#include "allocator.h"
#include "mylist.h"


constexpr auto factorial(auto n) -> decltype(n)
{
	return n ? n*factorial(n-1):1;
}

static_assert(factorial(9) == 9*8*7*6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(8) ==   8*7*6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(7) ==     7*6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(6) ==       6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(5) ==         5*4*3*2*1, "factorial failed!");
static_assert(factorial(4) ==           4*3*2*1, "factorial failed!");
static_assert(factorial(3) ==             3*2*1, "factorial failed!");
static_assert(factorial(2) ==               2*1, "factorial failed!");
static_assert(factorial(1) ==                 1, "factorial failed!");
static_assert(factorial(0) ==                 1, "factorial failed!");


namespace my
{
}


int main (int argc, char *argv[])
{

	my::my_logger = spdlog::basic_logger_st("logger", "allocator.log");
	my::my_logger->set_level(spdlog::level::trace);
	my::my_logger->info("--------- my::alloc_counter={}", my::alloc_counter);
	SPDLOG_TRACE(my::my_logger, "--------- my::alloc_counter={}", my::alloc_counter);


	auto make_factorial_value = [i=0] () mutable
	{
		auto f = factorial(i);
		auto value = std::make_pair(i,f);
		++i;
		return value;
	};


	std::map<int, int> m1;
	std::generate_n( std::inserter(m1, std::begin(m1))
				   , 10
				   , make_factorial_value
				   );


	std::map<int, int, std::less<int>, my::logging_allocator<std::pair<const int, int>, 10> > m2;
	std::generate_n( std::inserter(m2, std::begin(m2))
				   , 10
				   , make_factorial_value
				   );

	for(auto it: m2)
	{
		std::cout << it.first << " " << it.second << std::endl;
	}


	my::mylist<int> m3;
	for(size_t i=0; i<10; i++)
	{
		m3.append(i);
	}

	my::mylist<int, my::logging_allocator<int, 10>> m4;

	for(size_t i=0; i<10; i++)
	{
		m4.append(i);
	}

	for(auto it: m4)
	{
		std::cout << it << std::endl;
	}

	my::my_logger->info("========== my::alloc_counter={}", my::alloc_counter);
	SPDLOG_TRACE(my::my_logger, "========== my::alloc_counter={}", my::alloc_counter);
	return 0;
}

