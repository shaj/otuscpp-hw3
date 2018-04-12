
/**
	\file
	Модульные тесты
*/	




#include "lib.h"

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

/**
	Основной набор основных тестов 
*/
BOOST_AUTO_TEST_SUITE(test_suite_main)

/// Набор тестов для проверки генерации версии приложения
BOOST_AUTO_TEST_CASE(test_version_valid)
{
	BOOST_CHECK( version_major() == 0 );
	BOOST_CHECK( version_minor() == 0 );
	BOOST_CHECK( version_patch() > 0 );
}

BOOST_AUTO_TEST_SUITE_END()
