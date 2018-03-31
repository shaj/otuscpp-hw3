


#include <ios>
#include <iostream>
#include <vector>
#include <map>

#include "allocator_p.h"
#include "mylist.h"


// #define BOOST_LOG_DYN_LINK 1

// #include <boost/log/trivial.hpp>
// #include <boost/log/utility/setup/file.hpp>
// #include <boost/log/utility/setup/common_attributes.hpp>


// namespace logging = boost::log;
// namespace src = boost::log::sources;
// namespace sinks = boost::log::sinks;
// namespace keywords = boost::log::keywords;



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


	// logging::add_common_attributes();

	// logging::add_file_log(
	// 		keywords::file_name = "allocator.log",
	// 		keywords::rotation_size = 10 * 1024 * 1024,
	// 		keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0,0,0),
	// 		keywords::format = "[%TimeStamp%]: %Message%",
	// 		keywords::auto_flush = true,
	// 		keywords::open_mode = std::ios_base::app);


	// BOOST_LOG_TRIVIAL(info) << "Start allocator test";



/*	
	BOOST_LOG_TRIVIAL(info) << "Test vector with logging_allocator";
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


	// BOOST_LOG_TRIVIAL(info) << "Test map with std::allocator";
	auto m1 = std::map<int, long double>{};
	for(size_t i=0; i<10; i++)
	{
		m1[i] = fact(i);
	}
	for(auto it: m1)
	{
		std::cout << it.first << " " << it.second << std::endl;
	}

	// BOOST_LOG_TRIVIAL(info) << "Test map with logging_allocator";
	auto m2 = std::map<int, long double, std::less<int>,
			logging_allocator<std::pair<const int, long double>, 6>> {};

	for(size_t i=0; i<10; ++i)
	{
		m2[i] = fact(i);
	}
	for(auto it: m2)
	{
		std::cout << it.first << " " << it.second << std::endl;
	}


	// BOOST_LOG_TRIVIAL(info) << "Test mylist with std::allocator";
	// auto m3 = mylist<int>{};
	// for(size_t i=0; i<10; i++)
	// {
	// 	m3.append(i);
	// }
	// for(auto it: m3)
	// {
	// 	std::cout << it << std::endl;
	// }


	// BOOST_LOG_TRIVIAL(info) << "Test mylist with logging_allocator";
	auto m4 = mylist<int, logging_allocator<int, 6>>{};
	for(size_t i=0; i<10; i++)
	{
		m4.append(i);
	}
	for(auto it: m4)
	{
		std::cout << it << std::endl;
	}


	return 0;
}

