cmake_minimum_required(VERSION 3.2)

project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Boost COMPONENTS log REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Threads)

add_executable(allocator main.cpp newdelete.cpp)

add_executable(test_allocator test_allocator.cpp newdelete.cpp)


set_target_properties(allocator test_allocator PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_DEFINITIONS BOOST_LOG_DYN_LINK
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(allocator
  ${Boost_LIBRARIES}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_LOG_LIBRARY}
  ${Boost_LOG_SETUP_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties(test_allocator PROPERTIES
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
  INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)


target_link_libraries(test_allocator
  ${Boost_UNIT_TEST_LIBRARIES}
  ${Boost_LIBRARIES}
)


configure_file("${PROJECT_SOURCE_DIR}/version.h.in" "${PROJECT_BINARY_DIR}/version.h")
target_include_directories(allocator PUBLIC "${PROJECT_BINARY_DIR}")


install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT kolobrod2002@yandex.ru)

include (CPack)

enable_testing()

add_test(test_allocator test_allocator)
