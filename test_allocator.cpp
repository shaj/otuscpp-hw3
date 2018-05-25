
/**
	\file
	Модульные тесты
*/	

#define BOOST_LOG_DYN_LINK 1

#define BOOST_TEST_MODULE test_allocator

#include <boost/test/included/unit_test.hpp>

#include "allocator.h"
#include "mylist.h"
#include "newdelete.h"


/**
	Основной набор основных тестов 
*/
BOOST_AUTO_TEST_SUITE(test_allocator_suite)

BOOST_AUTO_TEST_CASE(test_logging_allocator) 
{
	my::logging_allocator<int, 10> a;

	auto ptr = a.allocate(1);
	BOOST_CHECK(ptr != nullptr);

	a.deallocate(ptr, 1);
}



/// Проверка работы односвязного списка с пользовательским аллокатором
BOOST_AUTO_TEST_CASE(test_mylist)
{
	my::mylist<int, my::logging_allocator<int, 10>> m1;
	for(size_t i=0; i<10; i++)
	{
		m1.append(i);
	}

	int cnt = 0;
	for(auto it: m1)
	{
		BOOST_CHECK(it == cnt);
		cnt++;
	}
}

/// Проверка основного условия задачи
BOOST_AUTO_TEST_CASE(test_main)
{

	std::size_t alloc_cnt {my::alloc_counter};

	my::mylist<int, my::logging_allocator<int, 10>> m1;
	for(size_t i=0; i<10; i++)
	{
		m1.append(i);
	}

	BOOST_CHECK((my::alloc_counter - alloc_cnt) <= 2);

}

/// Проверка освобождения памяти
BOOST_AUTO_TEST_CASE(test_memory_deallocate_1)
{

	std::size_t alloc_cnt {my::alloc_counter};

	// my::mylist<int, my::logging_allocator<int, 10>> m1;
	auto pm1 = new (my::mylist<int, my::logging_allocator<int, 10>>);
	for(size_t i=0; i<100; i++)
	{
		pm1->append(i);
	}

	delete pm1;

	BOOST_CHECK(my::alloc_counter == alloc_cnt);

}


BOOST_AUTO_TEST_SUITE_END()
