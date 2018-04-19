

#define BOOST_LOG_DYN_LINK 1

#include "newdelete.h"

#include <ios>
#include <iostream>
#include <vector>
#include <map>

#include "allocator.h"
#include "mylist.h"


#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;



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



int main (int argc, char *argv[])
{

	std::cout << "--------- my::alloc_counter=" << my::alloc_counter << std::endl;

	// logging::add_common_attributes();

	// logging::add_file_log(
	// 		keywords::file_name = "allocator.log",
	// 		keywords::rotation_size = 10 * 1024 * 1024,
	// 		keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0,0,0),
	// 		keywords::format = "[%TimeStamp%]: %Message%",
	// 		keywords::auto_flush = true,
	// 		keywords::open_mode = std::ios_base::app);

	// logging::core::get()->set_logging_enabled(false);	



	// po::options_description descr("Allowed options");
	// descr.add_options()
	// 	("help,h", "produce help message")
	// 	("version,v", "version")
	// 	("debug,d", "enable loggigng")
	// ;

	// po::variables_map vm;
	// po::store(po::parse_command_line(argc, argv, descr), vm);
	// po::notify(vm);

	// if(vm.count("help"))
	// {
	// 	std::cout << descr << std::endl;
	// 	return 0;
	// }

	// if(vm.count("version"))
	// {
	// 	std::cout << "version()" << std::endl;
	// 	return 0;
	// }

	// if(vm.count("debug"))
	// {
	// 	logging::core::get()->set_logging_enabled(true);	
	// }


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
	auto make_factorial_value = [i=0] () mutable
	{
		auto f = factorial(i);
		std::cout << i << " " << f << std::endl;
		auto value = std::make_pair(i,f);
		++i;
		return value;
	};


	// BOOST_LOG_TRIVIAL(info) << "Test map with std::allocator";
	std::map<int, int> m1;
	std::generate_n( std::inserter(m1, std::begin(m1))
				   , 15
				   , make_factorial_value
				   );

	for(auto it: m1)
	{
		std::cout << it.first << " " << it.second << std::endl;
	}

	// BOOST_LOG_TRIVIAL(info) << "Test map with logging_allocator";
	std::map<int, int, std::less<int>, my::logging_allocator<std::pair<const int, int>, 10> > m2;
	std::generate_n( std::inserter(m2, std::begin(m2))
				   , 15
				   , make_factorial_value
				   );

	for(auto it: m2)
	{
		std::cout << it.first << " " << it.second << std::endl;
	}


	// BOOST_LOG_TRIVIAL(info) << "Test mylist with std::allocator";
	// auto m3 = my::mylist<int>{};
	// for(size_t i=0; i<10; i++)
	// {
	// 	m3.append(i);
	// }

	// std::generate_n( std::inserter(m3, std::begin(m3))
	// 			   , 15
	// 			   , [i=0]()mutable{return i++;}
	// 			   );

	// for(auto it: m3)
	// {
	// 	std::cout << it << std::endl;
	// }


	// BOOST_LOG_TRIVIAL(info) << "Test mylist with logging_allocator";
	my::mylist<int, my::logging_allocator<int, 10>> m4;

	for(size_t i=0; i<10; i++)
	{
		m4.append(i);
	}

	// std::generate_n( std::inserter(m4, std::begin(m4))
	//                , 15
	//                , [i=0]()mutable{return i++;}
	//                );

	for(auto it: m4)
	{
		std::cout << it << std::endl;
	}



	std::cout << "========== my::alloc_counter=" << my::alloc_counter << std::endl;
	return 0;
}

